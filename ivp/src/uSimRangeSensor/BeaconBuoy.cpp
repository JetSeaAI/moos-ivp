/************************************************************/
/*    NAME: Michael Benjamin                                */
/*    ORGN: MIT Dept. of Mechanical Engineering             */
/*    FILE: BeaconBuoy.cpp                                  */
/*    DATE: Feb 1st, 2011                                   */
/************************************************************/

#include "BeaconBuoy.h"
#include "MBUtils.h"
#include "ColorParse.h"

using namespace std;

//-----------------------------------------------------------
// Constructor

BeaconBuoy::BeaconBuoy()
{
  m_x = 0;
  m_y = 0;
  m_timestamp = 0;
  m_report_range = 100;

  // resonable default values
  m_freq_low   = 60;
  m_freq_hgh   = 60;
  m_frequency  = 45;

  m_width      = 2;
  m_shape      = "circle";
  m_buoy_color = "green";
}


//-----------------------------------------------------------
// Procedure: setFrequencyRange
//  Examples: "0:10", "5", "40:100"

bool BeaconBuoy::setFrequencyRange(string str)
{
  if(tolower(str) == "never") {
    m_freq_low = -1;
    m_freq_hgh = -1;
    return(true);
  }

  string low = biteStringX(str, ':');
  string hgh = str;

  double lval = atof(low.c_str());
  double hval = atof(hgh.c_str());

  if(hgh == "") {
    double lval = atof(low.c_str());
    if(!isNumber(low) || (lval < 0))
      return(false);
    else 
      hval = lval;
  }
  else {
    if(!isNumber(low) || !isNumber(hgh))
      return(false);
    if((lval < 0) || (hval < 0))
      return(false);
    if(low > hgh) 
      return(false);
  }
  
  m_freq_low = lval;
  m_freq_hgh = hval;
  resetFrequency();
  return(true);
}

//-----------------------------------------------------------
// Procedure: setTimeStamp

bool BeaconBuoy::setTimeStamp(double timestamp)
{
  if(timestamp < m_timestamp)
    return(false);
  
  m_timestamp = timestamp;
  resetFrequency();
  return(true);
}

//-----------------------------------------------------------
// Procedure: setShape

bool BeaconBuoy::setShape(string shape_str)
{
  shape_str = tolower(shape_str);
  if((shape_str == "circle") || (shape_str == "diamond") ||
     (shape_str == "square") || (shape_str == "triangle") ||
     (shape_str == "efield") || (shape_str == "gateway")) {
    m_shape = shape_str;
    return(true);
  }
  return(false);
}

//-----------------------------------------------------------
// Procedure: setReportRange
//      Note: A negative range is allowed, but may be interpreted
//            as range = infinity.

bool BeaconBuoy::setReportRange(double range)
{
  m_report_range = range;
  return(true);
}


//-----------------------------------------------------------
// Procedure: setWidth

bool BeaconBuoy::setWidth(double width)
{
  if(width < 0)
    return(false);
  m_width = width;
  return(true);
}


//-----------------------------------------------------------
// Procedure: setBuoyColor()
// Procedure: setPulseLineColor()
// Procedure: setPulseFillColor()

bool BeaconBuoy::setBuoyColor(string color_str)
{
  if(!isColor(color_str))
    return(false);
  m_buoy_color = color_str;
  return(true);
}

bool BeaconBuoy::setPulseLineColor(string color_str)
{
  if(!isColor(color_str))
    return(false);
  m_pulse_line_color = color_str;
  return(true);
}

bool BeaconBuoy::setPulseFillColor(string color_str)
{
  if(!isColor(color_str))
    return(false);
  m_pulse_fill_color = color_str;
  return(true);
}

//-----------------------------------------------------------
// Procedure: getSpec()
//      Note: Returning the "full_spec" string includes features
//            of the beacon that do not have meaning for rendering.


string BeaconBuoy::getSpec(bool full_spec) const
{
  string str;
  str += "x=" + doubleToStringX(m_x);
  str += ",y=" + doubleToStringX(m_y);
  if(m_label != "")
    str += ",label=" + m_label;
  if(m_buoy_color != "")
    str += ",color=" + m_buoy_color;
  if(m_shape != "")
    str += ",type=" + m_shape;
  if(m_width > 0)
    str += ",width=" + doubleToStringX(m_width);
  if(full_spec) {
    str += ",freq=" + doubleToStringX(m_frequency);
    str += ",freq_low=" + doubleToStringX(m_freq_low);
    str += ",freq_hgh=" + doubleToStringX(m_freq_hgh);
    str += ",report_range=" + doubleToStringX(m_report_range);
  }
  
  return(str);
}

//-----------------------------------------------------------
// Procedure: resetFrequency
//      Note: A frequency of -1 means the beacon should never 
//            produce an uncolicited range report.

void BeaconBuoy::resetFrequency()
{
  if((m_freq_low < 0) || (m_freq_hgh < 0) || (m_freq_hgh < m_freq_low)) {
    m_frequency = -1;
    return;
  }

  double range = m_freq_hgh - m_freq_low;
  if(range <= 0)
    m_frequency = m_freq_low;
  
  int    rand_int = rand() % 1000;
  double rand_pct = (double)(rand_int) / 1000;
  
  m_frequency = m_freq_low + (rand_pct * range);
}

