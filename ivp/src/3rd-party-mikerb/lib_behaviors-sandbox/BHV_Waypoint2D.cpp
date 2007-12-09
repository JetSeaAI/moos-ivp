/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_Waypoint.cpp                                     */
/*    DATE: Jan 4th, 2006                                        */
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
#include <math.h> 
#include "BHV_Waypoint2D.h"
#include "AOF_Waypoint2D.h"
#include "OF_Reflector.h"
#include "BuildUtils.h"

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_Waypoint2D::BHV_Waypoint2D(IvPDomain gdomain) : IvPBehavior(gdomain)
{
  this->setParam("descriptor", "(d)bhv_waypoint2D");
  this->setParam("unifbox", "course=3, speed=2");
  this->setParam("gridbox", "course=6, speed=4");

  domain = subDomain(domain, "course,speed");

  current_waypt   = 0;
  arrival_radius  = 10; // Meters
  cruise_speed    = 0;  // Meters/second

  info_vars.push_back("NAV_X");
  info_vars.push_back("NAV_Y");
}

//-----------------------------------------------------------
// Procedure: setParam
//     Notes: We expect the "waypoint" entries will be of the form
//            "xposition,yposition".
//            The "radius" parameter indicates what it means to have
//            arrived at the waypoint.

bool BHV_Waypoint2D::setParam(string param, string val) 
{
  if(IvPBehavior::setParamCommon(param, val))
    return(true);

  if(param == "points") {
    seglist.initialize(val);
    return(true);
  }
  else if(param == "speed") {
    cruise_speed = atof(val.c_str());
    return(cruise_speed > 0);
  }
  else if(param == "radius") {
    arrival_radius = atof(val.c_str());
    return(arrival_radius >= 0);
  }
  return(false);
}


//-----------------------------------------------------------
// Procedure: produceOF

IvPFunction *BHV_Waypoint2D::produceOF() 
{
  if(!unif_box || !grid_box) {
    postEMessage("Null UnifBox or GridBox.");
    return(0);
  }
 
  bool ok1, ok2;
  double osX = info_buffer->dQuery("NAV_X", ok1);
  double osY = info_buffer->dQuery("NAV_Y", ok2);

  // Must get ownship position from InfoBuffer
  if(!ok1 || !ok2) {
    postEMessage("Missing critical ownship info in info_buffer.");
    return(0);
  }

  double ptX  = seglist.get_vx(current_waypt);
  double ptY  = seglist.get_vy(current_waypt);
  double dist = hypot((osX-ptX),(osY-ptY));
  if(dist < arrival_radius) {
    int vsize = seglist.size();
    if(current_waypt < (vsize-1)) {
      current_waypt++;
      ptX = seglist.get_vx(current_waypt);
      ptY = seglist.get_vy(current_waypt);
    }
    else {
      postMessage("VEHICLE_WPT_STAT_US", "complete");
      postMessage("VEHICLE_WPT_STAT", "complete");
      postFlags(end_flags);
      m_completed = true;
      return(0);
    }
  }

  //AOF_Waypoint2D aof_wpt(domain, cruise_speed, osY, osX, ptY, ptX);
  AOF_Waypoint2D aof_wpt(domain);
  aof_wpt.setParam("speed", cruise_speed);
  aof_wpt.setParam("oslat", osY);
  aof_wpt.setParam("oslon", osX);
  aof_wpt.setParam("ptlat", ptY);
  aof_wpt.setParam("ptlon", ptX);
  aof_wpt.initialize();

  OF_Reflector reflector(&aof_wpt);
  reflector.createUniform(unif_box);

  IvPFunction *ivp_function = reflector.extractOF();

  ivp_function->setPWT(priority_wt);

  return(ivp_function);
}






