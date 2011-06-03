/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: ContactRecord.h                                      */
/*    DATE: Feb 27th 2010                                        */
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

#ifndef CONTACT_RECORD_HEADER
#define CONTACT_RECORD_HEADER

#include <string>

class ContactRecord
{
 public:
  ContactRecord(const std::string& name);
  ~ContactRecord() {};

  bool valid();

  void setX(double v)         {m_x=v; m_x_set=true;};
  void setY(double v)         {m_y=v; m_y_set=true;};
  void setLat(double v)       {m_lat=v; m_lat_set=true;};
  void setLon(double v)       {m_lon=v; m_lon_set=true;};
  void setHeading(double v)   {m_heading=v; m_heading_set=true;};
  void setSpeed(double v)     {m_speed=v; m_speed_set=true;};
  void setDepth(double v)     {m_depth=v; m_depth_set=true;};
  void setTimeStamp(double v) {m_timestamp=v; m_timestamp_set=true;};
  void setType(std::string s) {m_type=s; m_type_set=true;};

  double getX(double timestamp=0);
  double getY(double timestamp=0);
  double getLat(double timestamp=0);
  double getLon(double timestamp=0);

  double getHeading()   {return(m_heading);};
  double getSpeed()     {return(m_speed);};
  double getDepth()     {return(m_depth);};
  double getTimeStamp() {return(m_timestamp);};

  std::string getName() {return(m_name);};
  std::string getType() {return(m_type);};

  std::string getValue(std::string, double=0);

 protected: 
  double m_x;
  double m_y;
  double m_speed;
  double m_heading;
  double m_depth;
  double m_lat;
  double m_lon;
  double m_timestamp;
  std::string  m_name;
  std::string  m_type;
  
  bool m_x_set;
  bool m_y_set;
  bool m_lat_set;
  bool m_lon_set;
  bool m_speed_set;
  bool m_heading_set;
  bool m_depth_set;
  bool m_type_set;
  bool m_timestamp_set;

  bool m_valid;
};

#endif 

