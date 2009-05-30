 /*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: XYBuildUtils.cpp                                     */
/*    DATE: Mar 16, 2008 Sunday Afternoon at Brugger's           */
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

#include <vector>
#include "XYFormatUtilsPoint.h"
#include "MBUtils.h"

using namespace std;

//---------------------------------------------------------------
// Procedure: string2Point
//

XYPoint string2Point(string str)
{
  if(strContains(str, "x=") && strContains(str, "y="))
    return(stringPairs2Point(str));
  else
    return(stringShort2Point(str));
}

//---------------------------------------------------------------
// Procedure: stringPairs2Point
//

XYPoint stringPairs2Point(string str)
{
  XYPoint null_point;

  str = tolower(stripBlankEnds(str));
  vector<string> mvector = parseString(str, ',');
  int vsize = mvector.size();
  
  // Below are the mandatory parameters - check they are set.
  bool x_set  = false;
  bool y_set  = false;

  string label, label_color, point_color, type, source;
  double x, y, z = 0;
  double snap    = 0;
  double size    = 1;
  bool   active  = true;

  for(int i=0; i<vsize; i++) {
    vector<string> svector = parseString(mvector[i], '=');
    if(svector.size() != 2)
      return(null_point);
    string param = stripBlankEnds(svector[0]);
    string value = stripBlankEnds(svector[1]);
    double dval  = atof(value.c_str());
    if((param == "x") && isNumber(value)) {
      x_set = true;
      x = dval;
    }
    else if((param == "y") && isNumber(value)) {
      y_set = true;
      y = dval;
    }
    else if((param == "z") && isNumber(value))
      z = dval;
    else if((param == "size") && isNumber(value))
      size = dval;
    else if((param == "snap") && (isNumber(value))) {
      if(dval >= 0)
	snap = dval;
    }
    else if(param == "label")
      label = value;
    else if(param == "label_color")
      label_color = value;
    else if(param == "point_color")
      point_color = value;
    else if(param == "source")
      source = value;
    else if(param == "type")
      type = value;
    else if(param == "active")
      active = (tolower(value) == "true");
  }

  if(!x_set || !y_set)
    return(null_point);
  
  XYPoint new_point;
  
  new_point.set_vertex(x,y,z);
  new_point.set_size(size);
  new_point.set_active(active);

  if(label != "")
    new_point.set_label(label);
  if(label_color != "")
    new_point.set_label_color(label_color);
  if(point_color != "")
    new_point.set_point_color(point_color);
  if(type != "")
    new_point.set_type(type);
  if(source != "")
    new_point.set_source(source);
  if(snap>=0)
    new_point.apply_snap(snap);
  
  return(new_point);
}

//---------------------------------------------------------------
// Procedure: stringShort2Point
//

XYPoint stringShort2Point(string str)
{
  XYPoint null_point;
  XYPoint new_point;

  str = tolower(stripBlankEnds(str));
  vector<string> mvector = parseString(str, ':');
  unsigned int i, vsize = mvector.size();
  
  for(i=0; i<vsize; i++) {
    mvector[i] = stripBlankEnds(mvector[i]);
    string left = tolower(stripBlankEnds(biteString(mvector[i], ',')));
    string rest = stripBlankEnds(mvector[i]);
    
    if(left == "label") 
      new_point.set_label(rest);
    else if(left == "label_color") 
      new_point.set_label_color(rest);
    else if(left == "type") 
      new_point.set_type(rest);
    else if(left == "source") 
      new_point.set_source(rest);
    else if(left == "time") 
      new_point.set_time(atof(rest.c_str()));
    else if(left == "point_color") 
      new_point.set_point_color(rest);
    else if(left == "active") 
      new_point.set_active(tolower(rest)=="true");
    else if(left == "size") 
      new_point.set_size(atof(rest.c_str()));
    else {
      string xstr = left;
      string ystr = stripBlankEnds(biteString(rest, ','));
      string zstr = stripBlankEnds(rest);
      if((zstr != "") && !isNumber(zstr))
	return(null_point);
      if(!isNumber(xstr) || !isNumber(ystr))
	return(null_point);
      double xval = atof(xstr.c_str());
      double yval = atof(ystr.c_str());
      if(zstr == "")
	new_point.set_vertex(xval, yval);
      else {
	double zval = atof(zstr.c_str());
	new_point.set_vertex(xval, yval, zval);	
      }
    }
  }
  return(new_point);
}
