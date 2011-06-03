/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: BehaviorRecord.h                                     */
/*    DATE: Mar 21st 2011                                        */
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

#ifndef BEHAVIOR_RECORD_HEADER
#define BEHAVIOR_RECORD_HEADER

#include <vector>
#include <string>

class BehaviorRecord
{
public:
  BehaviorRecord();
  virtual ~BehaviorRecord() {};

  // Setters
  void   setTimeStamp(double v);
  void   setName(std::string s)      {m_bhv_name = s;};
  void   setPriority(std::string s)  {m_priority = s;};
  void   setTimeCPU(std::string s)   {m_time_cpu = s;};
  void   setPieces(std::string s)    {m_pieces = s;};
  void   setUpdates(std::string s)   {m_updates = s;};

  // Getters
  double getTimeStamp() const       {return(m_time_stamp);};
  std::string getName() const       {return(m_bhv_name);};
  std::string getTimeCPU() const    {return(m_time_cpu);};
  std::string getPriority() const   {return(m_priority);};
  std::string getPieces() const     {return(m_pieces);};
  std::string getUpdates() const    {return(m_updates);};

  std::string getSummary(double tstamp=0) const;

protected:
  double      m_time_stamp;
  bool        m_set_time_stamp;

  std::string m_bhv_name;
  std::string m_time_cpu;
  std::string m_priority;
  std::string m_pieces;
  std::string m_updates;

};

#endif 

