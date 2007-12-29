/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_WaypointBB.cpp                                   */
/*    DATE: Nov 2004                                             */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/

#ifdef _WIN32
#pragma warning(disable : 4786)
#pragma warning(disable : 4503)
#endif
#include <iostream>
#include <math.h> 
#include "BHV_WaypointB.h"
#include "ZAIC_PEAK.h"
#include "ZAIC_PEAK.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "GeomUtils.h"
#include "BuildUtils.h"
#include "FunctionEncoder.h"
#include "OF_Coupler.h"

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_WaypointB::BHV_WaypointB(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  this->setParam("descriptor", "(d)bhv_waypoint");

  m_domain = subDomain(m_domain, "course,speed");

  current_waypt   = 0;
  arrival_radius  = 10; // Meters
  cruise_speed    = 0;  // Meters/second
  nm_radius       = 0;  // Meters
  current_cpa     = 100000;
  lead_distance   = -1;
  repeat          = 0;

  m_completed     = false;

  osX   = -1;
  osY   = -1;
  osSPD = -1;
  ptX   = -1;
  ptY   = -1;
  iptX  = -1;
  iptY  = -1;

  addInfoVars("NAV_X, NAV_Y, NAV_SPEED");
}

//-----------------------------------------------------------
// Procedure: setParam
//     Notes: We expect the "waypoint" entries will be of the form
//            "xposition,yposition".
//            The "radius" parameter indicates what it means to have
//            arrived at the waypoint.

bool BHV_WaypointB::setParam(string param, string val) 
{
  if(IvPBehavior::setParam(param, val))
    return(true);

  if((param == "polygon") || (param == "points")) {
    XYSegList new_seglist;
    bool ok = new_seglist.initialize(val);
    if(!ok)
      return(false);
    seglist = new_seglist;
    return(true);
  }
  else if(param == "speed") {
    double dval = atof(val.c_str());
    if((dval <= 0) || (!isNumber(val)))
      return(false);
    cruise_speed = dval;
    return(true);
  }
  else if(param == "lead") {
    double dval = atof(val.c_str());
    if((dval < 0) || (!isNumber(val)))
      return(false);
    lead_distance = dval;
    return(true);
  }
  else if(param == "order") {
    if((val!="reverse") && (val!="reversed") && (val!="normal"))
      return(false);
    if((val == "reverse") || (val == "reversed"))
      seglist.reverse();
    return(true);
  }
  else if(param == "repeat") {
    int ival = atoi(val.c_str());
    if((ival < 0) || (!isNumber(val)))
      return(false);
    repeat = ival;
    return(true);
  }
  else if(param == "radius") {
    double dval = atof(val.c_str());
    if(dval <= 0)
      return(false);
    arrival_radius = dval;
    return(true);
  }
  else if(param == "nm_radius")  {
    double dval = atof(val.c_str());
    if(dval <= 0) 
      return(false);
    nm_radius = dval;
    return(true);
  }
  return(false);
}


//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_WaypointB::onRunState() 
{
  // Set osX, osY, ptX, ptY, iptX, iptY;
  bool valid_point = setNextWaypoint();

  // If no next-waypoint return NULL - it may or may not be an
  // error condition. That is determined in the setNextWaypoint()
  // function.
  if(!valid_point)
    return(0);

  ZAIC_PEAK spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(ptSPD);
  spd_zaic.setBaseWidth(0.3);
  spd_zaic.setPeakWidth(0.0);
  spd_zaic.setSummitDelta(0.0);

  IvPFunction *spd_of = spd_zaic.extractOF();

  double rel_ang_to_wpt = relAng(osX,osY,ptX,ptY);

  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(rel_ang_to_wpt);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setValueWrap(true);

  IvPFunction *crs_of = crs_zaic.extractOF();

  OF_Coupler coupler;
  IvPFunction *ipf = coupler.couple(crs_of, spd_of);

  if(ipf)
    ipf->setPWT(m_priority_wt);

#if 0
  IvPBox mpt = ipf->getPDMap()->getGrid()->getMaxPt();
  cout << "BHV_WaypointB::produceOF():" << endl;
  cout << "maxpt:" << endl;
  mpt.print();
#endif

  if(osSPD > 0) {
    double dist_meters = hypot((osX-ptX), (osY-ptY));
    double eta_seconds = dist_meters / osSPD;

    string stat = "vname=" + m_us_name + ",";
    stat += "index=" + intToString(current_waypt)   + ",";
    stat += "dist="  + doubleToString(dist_meters)  + ",";
    stat += "eta="   + doubleToString(eta_seconds);
    
    postMessage("WAYPOINT_STATUS", stat);
    postMessage("WAYPOINT_INDEX", current_waypt);
  }

  postMessage("VIEW_POLYGON", seglist.get_spec());

  string str = IvPFunctionToString(ipf);
  postMessage("BHV_IPF", str);

  return(ipf);
}

//-----------------------------------------------------------
// Procedure: setNextWaypoint

bool BHV_WaypointB::setNextWaypoint()
{
  bool ok1, ok2, ok3;
  osX   = m_info_buffer->dQuery("NAV_X",     ok1);
  osY   = m_info_buffer->dQuery("NAV_Y",     ok2);
  osSPD = m_info_buffer->dQuery("NAV_SPEED", ok3);

  // Must get ownship position from InfoBuffer
  if(!ok1 || !ok2) {
    postEMessage("No ownship X/Y info in info_buffer.");
    return(false);
  }
  if(!ok3)
    postWMessage("No ownship speed info in info_buffer.");

  int vsize = seglist.size();
  if(vsize == 0)
    return(false);

  ptX  = seglist.get_vx(current_waypt);
  ptY  = seglist.get_vy(current_waypt);
  double dist = hypot((osX-ptX),(osY-ptY));

  ptSPD = cruise_speed;
  
  if(dist < current_cpa)
    current_cpa = dist;

  // Determine if waypoint we had been seeking has been reached
  bool next_point = false;
  if(dist < arrival_radius)
    next_point = true;
  else {
    if((nm_radius > arrival_radius) &&
       (dist > current_cpa) &&
       (current_cpa <= nm_radius))
      next_point = true;
  }

  // If arrived at waypoint we had been seeking, determine if
  // (1) just increment the waypoint, or
  // (2) start repeating the waypoints, or
  // (3) declare completion of the behavior.
  if(next_point) {
    if(current_waypt < ((vsize-1))) {
      current_waypt++;
      if((current_waypt == (vsize-1)) && (repeat > 0)) {
	current_waypt = 0;
	repeat--;
      }
      ptX = seglist.get_vx(current_waypt);
      ptY = seglist.get_vy(current_waypt);
      current_cpa = hypot((osX-ptX),(osY-ptY));
    }
    else {
      postMessage("VEHICLE_WPT_STAT_US", "complete");
      postMessage("VEHICLE_WPT_STAT", "complete");
      postFlags(m_end_flags);
      m_completed = true;
      return(false);
    }
  }

  // By default, the steering point is the next waypoint.
  iptX = ptX;
  iptY = ptY;

  // If lead_distance is non-neg, and we've already hit the
  // first waypoint, then steer to an intermediate point that
  // is lead_distance away from the perpendicular intersection
  // point between the current position and the trackline.

  if(lead_distance >= 0) {
    if(current_waypt > 0) {
      double x1 = seglist.get_vx(current_waypt-1);
      double y1 = seglist.get_vx(current_waypt-1);

      double nx, ny;
      perpSegIntPt(x1,y1,ptX,ptY,osX,osY,nx,ny);

      double angle = relAng(x1,y1,ptX,ptY);
      double dist  = distPointToPoint(nx, ny, ptX, ptY);
      if(dist > lead_distance) 
	dist = lead_distance; 
      projectPoint(angle, dist, nx, ny, iptX, iptY);
    }
  }
    
#if 0
  cout << "current_waypt: " << current_waypt << endl;
  cout << "seglist.size(): " << seglist.size() << endl;
  cout << "   ptX:" <<  ptX << "  ptY:" << ptY  << endl;
  cout << "  iptX:" << iptX << " iptY:" << iptY << endl;
#endif

  return(true);
}








