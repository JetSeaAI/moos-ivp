/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: XYFormatUtilsPoly.h                                  */
/*    DATE: May 16, 2009 After bbq @ Hows                        */
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
 
#ifndef XY_FORMAT_UTILS_POLY_HEADER
#define XY_FORMAT_UTILS_POLY_HEADER

#include <string>
#include "XYPolygon.h"

//---------------------------------------------------------------
// Create a polygon from a string specification. This function will
// call one of the string*Poly(string) functions below.
XYPolygon string2Poly(std::string);

// Create a polygon that approximates an ellipse
// Example: "ellipse:: x=val, y=val, major=val, minor=val, 
//    degs=val, rads=val, pts=val, snap_value=val, label=val"
XYPolygon stringEllipse2Poly(std::string);

// Create a polygon that approximates radial pie wedge
// Example: "wedge:: x=val, y=val, lang=val, rang=val,
//    pts=val, range=val, snap_value=val, label=val"
XYPolygon stringPieWedge2Poly(std::string);

// Create a polygon that approximates radial range wedge
// Example: "type=wedge, x=val, y=val, lang=val, rang=val,
//    pts=val, range=val, snap_value=val, label=val"
XYPolygon stringRangeWedge2Poly(std::string);

// Create a rectangle from two given points
// Example: "x1=val, y1=val, x2=val, y2=val, axis_pad=val, 
//          perp_pad=val, snap_val=val, label=val"
XYPolygon stringPylon2Poly(std::string);

// Create a polygon that approximates a circle.
// Example: "radial:: x=val, y=val, radius=val, pts=val, snap=val, 
//    label=val"
XYPolygon stringRadial2Poly(std::string);

// Create a polygon that approximates a circle.
// Example: "radial:xval,yval,radius,pts,snapval,label 
XYPolygon stringShortRadial2Poly(std::string);

// Create a polygon given a set of points
// Examples: [pts="10,15:20,25:30,35", label=foobar]
//           [label=foobar, pts="10,15:20,25:30,35"]
//           [label=foobar, "10,15 : 20,25 : 30,35"]
XYPolygon stringPoints2Poly(std::string);
  
// Create a polygon given a set of points
// Examples: [10,15:20,25:30,35:label,foobar]
XYPolygon stringShortPoints2Poly(std::string);
  
#endif
