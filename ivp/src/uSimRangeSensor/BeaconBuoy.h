/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: BeaconBuoy.h                                         */
/*    DATE: Feb 1st, 2011                                        */
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

#ifndef BEACON_BUOY_HEADER
#define BEACON_BUOY_HEADER

#include <string>

class BeaconBuoy
{
 public:
  BeaconBuoy();
  virtual ~BeaconBuoy() {};

  // Setters
  void    setX(double x)               {m_x=x;};
  void    setY(double y)               {m_y=y;};
  void    setLabel(std::string label)  {m_label=label;};

  bool    setFrequencyRange(std::string);
  bool    setReportRange(double);
  bool    setShape(std::string);
  bool    setWidth(double);
  bool    setBuoyColor(std::string);
  bool    setPulseLineColor(std::string);
  bool    setPulseFillColor(std::string);
  
  // Getters
  double  getFrequency() const   {return(m_frequency);};
  double  getTimeStamp() const   {return(m_timestamp);};
  double  getX() const           {return(m_x);};
  double  getY() const           {return(m_y);};
  double  getReportRange() const {return(m_report_range);};

  std::string getLabel() const   {return(m_label);};
  std::string getSpec(bool=false) const;

 public: // Updating beacon history
  bool    setTimeStamp(double tstamp);

 protected:
  void    resetFrequency();

 protected: // Unique identifier
  std::string  m_label;

 protected: // Key properties
  double  m_x;
  double  m_y;
  double  m_report_range;
  double  m_frequency;
  double  m_timestamp;

  double  m_freq_low;
  double  m_freq_hgh;

 protected: // Drawing hints
  double       m_width;
  std::string  m_shape;
  std::string  m_buoy_color;
  std::string  m_pulse_line_color;
  std::string  m_pulse_fill_color;
};

#endif 

