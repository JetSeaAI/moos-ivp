/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_SentryOnce.cpp                                   */
/*    DATE: July 20th 2005 Pianosa                               */
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
#include "GeomUtils.h"
#include "BHV_SentryOnce.h"
#include "AOF_WPT2D.h"
#include "OF_Reflector.h"

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_SentryOnce::BHV_SentryOnce(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  this->setDescriptor("(d)bhv_sentry_once");
  this->setParam("build_info", "uniform_box=course:3,speed:3");
  this->setParam("build_info", "uniform_grid=course:9,speed:6");

  current_waypt  = 0;
  arrival_radius = 10; // Meters
  cruise_speed   = 0;  // Meters/second
  sentry_mode    = 1;  // Default is acquire mode
  range_max      = 0;
}

//-----------------------------------------------------------
// Procedure: setParam

bool BHV_SentryOnce::setParam(string g_param, string g_val) 
{
  if(IvPBehavior::setParamCommon(g_param, g_val))
    return(true);

  if(g_param == "polygon") {
    poly.clear();
    // Should be convex - init will return false otherwise
    return(poly.initialize(g_val));
  }  
  else if(g_param == "them") {
    them_name = g_val;
    return(true);
  }  
  else if(g_param == "clockwise") {
    clockwise = false;
    if(g_val == "true")
      clockwise = true;
    return((g_val == "true") || (g_val == "false"));
  }  
  else if(g_param == "speed") {
    cruise_speed = atof(g_val.c_str());
    return(cruise_speed > 0);
  }
  else if(g_param == "radius") {
    arrival_radius = atof(g_val.c_str());
    return(arrival_radius >= 0);
  }
  else if(g_param == "range_max") {
    range_max = atof(g_val.c_str());
    return(range_max >= 0);
  }  

  return(false);
}


//-----------------------------------------------------------
// Procedure: acquireVertex
//   Purpose: Determine which of the viewable vertices in the polygon
//            is the best point to serve as a vehicle entry point. 
//            Determination is based on which direction the vehicle
//            will be travelling around the polygon (clockwise or not).

int BHV_SentryOnce::acquireVertex(double os_x, double os_y, bool inside)
{
  
  int vsize = poly.size();
  if(vsize == 0)
    return(-1);

  // Special case - if only one vertex in the polygon, then that
  // can be the only vertex chosen. (It would be an odd polygon
  // for the Sentry behavior, but that is outside the scope here)
  if(vsize == 1)
    return(0);   

  int    index = -1;
  bool   fresh = true;
  double best_angle;
  for(int i=0; i<vsize; i++) {
    if(poly.vertex_is_viewable(i, os_x, os_y)) {
      int j = i+1;
      if(i == vsize-1)
	j = 0;
      double x2 = poly.get_vx(i);
      double y2 = poly.get_vy(i);
      double x3 = poly.get_vx(j);
      double y3 = poly.get_vy(j);
      double angle = segmentAngle(os_x, os_y, x2,y2,x3,y3);
      if(clockwise) {
	if(fresh || (angle > best_angle)) {
	  fresh = false;
	  index = i;
	  best_angle = angle;
	}
      }
      else {
	if(fresh || (angle < best_angle)) {
	  fresh = false;
	  index = i;
	  best_angle = angle;
	}
      }
    }
  }  
  return(index);
}


//-----------------------------------------------------------
// Procedure: onRunState

OF *BHV_SentryOnce::onRunState() 
{
  // Check for "pause" mode
  if(sentry_mode == -1) 
    return(0);
  
  bool ok1, ok2;
  // ownship position in meters from some 0,0 reference point.
  double osX = m_info_buffer->dQuery("NAV_X", &ok1);
  double osY = m_info_buffer->dQuery("NAV_Y", &ok2);

  // Must get ownship position from InfoBuffer
  if(!ok1 || !ok2) {
    postEMessage("No ownship info in info_buffer.");
    return(0);
  }

  double relevance = 1.0;
  if(them_name != "") {
    double cnY = m_info_buffer->dQuery(them_name, "NAV_Y", &ok1);
    double cnX = m_info_buffer->dQuery(them_name, "NAV_X", &ok2);
    // Must get them position from InfoBuffer
    if(!ok1 || !ok2) {
      postWMessage("BHV_SentryOnce: contact X/Y info not found.");
    }
    else {
      // Calculate the relevance first. If zero-relevance, we won't
      // bother to create the objective function.
      // Also, if found to be irrelevant, set the mode=1 so when it
      // becomes relevant again, it aquires the appropriate vertex.
      relevance = getRelevance(osX, osY, cnX, cnY);
      if(relevance <= 0.0) {
	postMessage("INZONE", "true"); 
	sentry_mode = 1;
	return(0);
      }
    }
  }

  // Check for "acquire" mode
  if(sentry_mode == 1)
    current_waypt = acquireVertex(osX, osY, 0); 

  if((current_waypt < 0) || 
     (current_waypt >= poly.size())) {
    postEMessage("Invalid waypoint determination");
    return(0);
  }

  double ptX = poly.get_vx(current_waypt);
  double ptY = poly.get_vy(current_waypt);

  // Check to see if we have hit the current/prior waypoint. 
  // If so, update the waypoint, and declare that we are back
  // in a "normal" mode of operation if we were not before.
  if(hypot((osX-ptX),(osY-ptY)) < arrival_radius) {
    sentry_mode = 0; // Normal mode
    current_waypt++;
    if(current_waypt >= poly.size())
      current_waypt = 0;
    ptX = poly.get_vx(current_waypt);
    ptY = poly.get_vy(current_waypt);
  }

  AOF_WPT2D    *aof_wpt = new AOF_WPT2D(m_domain, cruise_speed, osY, osX, ptY, ptX);
  OF_Reflector *ofr_wpt = new OF_Reflector(aof_wpt, 1);

  ofr_wpt->create(m_build_info);
  OF *of = ofr_wpt->extractOF();

  delete(ofr_wpt);

  of->setDomainName(0, "course");
  of->setDomainName(1, "speed");
  of->setPWT(relevance * m_priority_wt);

  return(of);
}

//-----------------------------------------------------------
// Procedure: getRelevance

double BHV_SentryOnce::getRelevance(double osX, double osY,
				double cnX, double cnY)
{
  double contact_range = hypot((osX-cnX), (osY-cnY));
  cout << "ContactRANGE:" << contact_range << endl;
  postMessage("CONTACT_RANGE", contact_range);
  if(contact_range < range_max)
    return(0.0);
  else
    return(1.0);
}





