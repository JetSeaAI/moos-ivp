/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: XYRangePulse.cpp                                     */
/*    DATE: Feb 4th 2011                                         */
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

#include "XYRangePulse.h"
#include "GeomUtils.h"
#include "ColorPack.h"

using namespace std;

//-------------------------------------------------------------
// Constructor

XYRangePulse::XYRangePulse()
{
  initialize();
}

XYRangePulse::XYRangePulse(double x, double y)
{
  initialize();
  m_x = x;
  m_y = y;
  m_x_set = true;
  m_y_set = true;
}

//-------------------------------------------------------------
// Procedure: initialize

void XYRangePulse::initialize()
{
  // Superclass member variables
  set_edge_size(1);
  set_color("edge", "green");
  set_color("fill", "green");
  
  // Local member variables
  m_x    = 0;
  m_y    = 0;
  m_rad  = 40;

  m_x_set = false;
  m_y_set = false;

  m_duration = 15;
  m_fill = 0.25;
}

//-------------------------------------------------------------
// Procedure: get_circle()

vector<double> XYRangePulse::get_circle(double timestamp, 
					unsigned int pts) const
{
  // Make sure minimally we are describing a triangle.
  if(pts < 3)
    pts = 3;

  vector<double> vpts;
  if((m_rad <= 0) || (m_duration <= 0))
    return(vpts);

  double elapsed_time = timestamp - m_time;

  if((elapsed_time < 0) || (elapsed_time > m_duration))
    return(vpts);

  double pct = 1.0;
  if(m_duration > 0) 
    pct = elapsed_time / m_duration;
  double range = pct * m_rad;

  double new_x, new_y;
  double delta = 360.0 / (double)(pts);
  for(double deg=(delta/2); deg<360; deg+=delta) {
    projectPoint(deg, range, m_x, m_y, new_x, new_y);
    vpts.push_back(new_x);
    vpts.push_back(new_y);
  }
  return(vpts);
}

//-------------------------------------------------------------
// Procedure: set_rad()

void XYRangePulse::set_rad(double val)
{
  m_rad = val;
  if(m_rad < 0)
    m_rad = 0;
}

//-------------------------------------------------------------
// Procedure: set_duration()

void XYRangePulse::set_duration(double val)
{
  m_duration = val;
  if(m_duration < 0)
    m_duration = 0;
}

//-------------------------------------------------------------
// Procedure: set_fill()

void XYRangePulse::set_fill(double val)
{
  if(val < 0)
    val = 0;
  if(val > 1)
    val = 1;

  m_fill = val;
}

//-------------------------------------------------------------
// Procedure: get_fill()

double XYRangePulse::get_fill(double timestamp) const
{
  double elapsed_time = timestamp - m_time;

  if(elapsed_time <= 0)
    return(m_fill);
  if(elapsed_time >= m_duration)
    return(0);

  double pct = 1.0;
  if(m_duration > 0) 
    pct = (1-(elapsed_time / m_duration));
  double rval = pct  * m_fill;
  return(rval);
}


//-------------------------------------------------------------
// Procedure: valid()

bool XYRangePulse::valid() const
{
  return(m_x_set && m_y_set);
}

//-------------------------------------------------------------
// Procedure: get_spec()

string XYRangePulse::get_spec(string param) const
{
  string spec = "x=";
  spec += doubleToStringX(m_x); 
  spec += ",y=";
  spec += doubleToStringX(m_y);
  spec += ",radius=";
  spec += doubleToStringX(m_rad); 
  spec += ",duration=";
  spec += doubleToStringX(m_duration); 
  spec += ",fill=";

  string obj_spec = XYObject::get_spec(param);
  if(obj_spec != "")
    spec += ("," + obj_spec);
  
  return(spec);
}


