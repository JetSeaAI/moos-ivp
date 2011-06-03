/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: VehicleSet.h                                         */
/*    DATE: Aug 12th 2008                                        */
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

#ifndef MARINE_VEHICLE_DATASET_HEADER
#define MARINE_VEHICLE_DATASET_HEADER

#include <vector>
#include <string>
#include <map>
#include "ObjectPose.h"
#include "ColoredPoint.h"
#include "ColorPack.h"
#include "BearingLine.h"

class VehicleSet
{
 public:
  VehicleSet();
  ~VehicleSet() {};
    
  bool   setParam(std::string param, std::string value="");
  bool   setParam(std::string param, double value);

  void   clear(const std::string& vname="");
  double getXMin() const {return(m_xmin);};
  double getXMax() const {return(m_xmax);};
  double getYMin() const {return(m_ymin);};
  double getYMax() const {return(m_ymax);};

  //------ General Purpose Information retrieval --------------
  bool  getDoubleInfo(const std::string& vname, 
		      const std::string& info_type, 
		      double& result) const;
  
  bool  getStringInfo(const std::string& vname, 
		      const std::string& info_type, 
		      std::string& result) const;

  //------ Simplified Information retrieval --------------
  std::string getStringInfo(const std::string& vname, 
			    const std::string& info_type) const;
  double      getDoubleInfo(const std::string& vname, 
			    const std::string& info_type) const;

  //------ Very Simplified Information retrieval --------------
  std::string getStringInfo(const std::string& info_type) const;
  double      getDoubleInfo(const std::string& info_type) const;

  std::vector<std::string> getVehiNames() const;

  std::string getActiveVehicle() const  {return(m_vehicles_active_name);};
  std::string getCenterVehicle() const  {return(m_vehicles_center_name);};

  CPList      getVehiHist(const std::string&   s="active") const;   
  ObjectPose  getObjectPose(const std::string& s="active") const;
  BearingLine getBearingLine(const std::string& s="active") const;

  bool  hasVehiName(const std::string&) const;
  bool  getWeightedCenter(double& x, double&y) const;

 private:
  bool  updateVehiclePosition(const std::string& node_report); 
  bool  updateVehicleBearingLine(const std::string& bearing_line); 

 private:
  // Mapping from Vehicle Name to Vehicle Position
  std::map<std::string, ObjectPose>   m_pos_map;
  // Mapping from Vehicle Name to Vehicle Length
  std::map<std::string, double>       m_vlen_map;
  // Mapping from Vehicle Name to Vehicle Position History
  std::map<std::string, CPList>       m_hist_map;
  // Mapping from Vehicle Name to Vehicle Body Type
  std::map<std::string, std::string>  m_vbody_map;
  // Mapping from Vehicle Name to Vehicle Helm Mode (in NODE_REPORT)
  std::map<std::string, std::string>  m_vmode_map;
  // Mapping from Vehicle Name to Vehicle Helm AllStop Mode (in NODE_REPORT)
  std::map<std::string, std::string>  m_amode_map;
  // Mapping from Vehicle Name to Time of last node report
  std::map<std::string, double>       m_ais_map;

  // Mapping from Vehicle Name to Bearing Lines
  std::map<std::string, BearingLine> m_bearing_map;

  std::string m_vehicles_active_name;
  std::string m_vehicles_center_name;

  unsigned int m_history_size;
  double       m_curr_time;

  double  m_xmin;
  double  m_xmax;
  double  m_ymin;
  double  m_ymax;
  
  std::vector<std::string> m_node_report_vars;
};

#endif 



