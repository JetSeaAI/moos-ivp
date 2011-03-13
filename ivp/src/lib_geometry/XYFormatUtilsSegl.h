/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: XYFormatUtilsSegl.h                                  */
/*    DATE: May 18th, 2009 (Reworked from XYBuildUtils.h)        */
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
 
#ifndef XY_FORMAT_UTILS_SEGLIST_HEADER
#define XY_FORMAT_UTILS_SEGLIST_HEADER

#include <string>
#include "XYPolygon.h"
#include "XYSegList.h"
#include "XYPoint.h"

//------------------------------------------------------------------
// #0 Create an XYSegList from a string specification. 
// This function will call one of the string*SegList(string) 
// functions below. This is the only function that should be called 
// by the user. The other functions are subject to change without 
// regard to backward compatibility.
XYSegList string2SegList(std::string);

// *** IMPORTANT NOTE **** 
// The below functions are not intended to be invoked directly by 
// the user. They are subortinate functions called by the above
// function. While the above function will be maintained in the 
// future for backward compatibility, the below functions are not.

//---------------------------------------------------------------
// #1 Create an XYSeglist from a string specification. 
// This function is standard because it processes the string format
// used when a string is created from an existing XYSegList instance.
// Example: pts=2,3:5,6:9,5
// Example: pts=2,3:5,6,foo:9,5::label=bobby, type=destination
XYSegList stringStandard2SegList(std::string);

//---------------------------------------------------------------
// #2 Create an XYeglist from a string specifcations
// Examples: 10,15
//           label,foobar : 10,15 : 20,25 : 30,35
XYSegList stringAbbreviated2SegList(std::string);

//---------------------------------------------------------------
// #3 Create an XYSegList from a string describing a zigzag pattern.
// Example: zigzag: startx, starty, angle, length, period, 
//          amplitude, snapval

XYSegList stringZigZag2SegList(std::string);

//---------------------------------------------------------------
// #4 Create a seglist given a set of points for a lawnmower pattern
// Example: "format=lawnmower, x=0, y=8, width=100, height=80, 
//          degs=45, swath=20, startx=-40, starty=80, start=tl"
XYSegList stringLawnmower2SegList(std::string);

#endif
