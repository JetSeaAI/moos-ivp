/************************************************************/
/*    NAME: Michael Benjamin, H.Schmidt, J.Leonard          */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge             */
/*    FILE: EnumVariable.cpp                                */
/*    DATE: Dec 26th 2009                                   */
/************************************************************/

#include <cstdlib>
#include "EnumVariable.h"
#include "MBUtils.h"

using namespace std;

//---------------------------------------------------------
// Constructor

EnumVariable::EnumVariable()
{
  // Initial values for state variables.
  m_index        = 0;
  m_timestamp    = 0;
  m_total_weight = 0;

  // Initial values for configuration parameters
  m_varname = "random_var";
  m_keyname = "";

  setRandomSeed();
}

//---------------------------------------------------------
// Procedure: addElement

void EnumVariable::addElement(string str, double weight)
{
  m_elements.push_back(str);
  if(weight < 0)
    weight = 0;
  m_weights.push_back(weight);
  m_total_weight += weight;
}

//---------------------------------------------------------
// Procedure: reset

string EnumVariable::reset(string key, double timestamp)
{
  if(m_keyname == key)  {
    if(m_timestamp >= 0)
      m_timestamp = timestamp;
    
    int    rand_int = rand() % 10000;
    double rand_pct = (double)(rand_int) / 10000;
    double wt_index = rand_pct * m_total_weight;
    
    double running_weight = 0;
    unsigned int i, vsize = m_weights.size();
    for(i=0; i<vsize; i++) {
      running_weight += m_weights[i]; 
      if(wt_index < running_weight)
	m_index = i;
    }
  }

  if(m_index >= m_elements.size())
    return("Error: Index Out of Range");
  else
    return(m_elements[m_index]);
}
  
//---------------------------------------------------------
// Procedure: getAge

double EnumVariable::getAge(double timestamp)
{
  if((m_timestamp < 0) || (timestamp < m_timestamp))
    return(0);
  else
    return(timestamp - m_timestamp);
}

 
//---------------------------------------------------------
// Procedure: getValue

string EnumVariable::getValue() const
{
  if(m_elements.size() == 0)
    return("empty");
  return(m_elements[m_index]);
}

  
//---------------------------------------------------------
// Procedure: getStringSummary

string EnumVariable::getStringSummary()
{
  string str = "varname=" + m_varname;
  if(m_keyname != "")
    str += ", keyname=" + m_keyname;
  str += ", elements={";
  unsigned int i, vsize = m_elements.size();
  for(i=0; i<vsize; i++) {
    if(i!=0)
      str += ",";
    str += m_elements[i];
    str += "[" + dstringCompact(doubleToString(m_weights[i])) + "]";
  }
  return(str);
}
  
  
//---------------------------------------------------------
// Procedure: setRandomSeed

void EnumVariable::setRandomSeed()
{
  unsigned long tseed = time(NULL) + 1;
  unsigned long pid = (long)getpid() + 1;
  unsigned long seed = (tseed%999999);
  seed = ((rand()) * seed) % 999999;
  seed = (seed * pid) % 999999;
  srand(seed);
}
  
