/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: CPAEngineX.h                                         */
/*    DATE: May 12th 2005                                        */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/
 
#ifndef CPA_ENGINE_X_HEADER
#define CPA_ENGINE_X_HEADER

#include <vector>

class CPAEngineX {
public:
  CPAEngineX();
  CPAEngineX(double cnY, double cnX, double cnh, double cnv, 
	     double osY, double osX);

  void reset(double cnY, double cnX, double cnh, double cnv, 
	     double osY, double osX);
  
  ~CPAEngineX() {}

public:    
  double evalCPA(double osh, double osv, double ostol, double* calc_roc=0) const;
  double evalROC(double osh, double osv) const;
  bool   crossesLines(double osh) const;

  bool   crossesStern(double osh, double osv) const;
  double crossesSternDist(double osh, double osv) const;
  bool   crossesSternDist(double osh, double osv, double& xdist) const;

  bool   crossesBow(double osh, double osv) const;
  double crossesBowDist(double osh, double osv) const;
  bool   crossesBowDist(double osh, double osv, double& xdist) const;

  bool   crossesBowOrStern(double osh, double osv) const;

  bool   turnsRight(double old_osh, double new_osh) const;
  bool   turnsLeft(double old_osh, double new_osh) const;

  bool   passesContact(double osh, double osv) const;
  bool   passesContactPort(double osh, double osv) const;
  bool   passesContactStarboard(double osh, double osv) const;
  bool   foreOfContact() const;
  bool   aftOfContact() const;
  bool   portOfContact() const;
  bool   starboardOfContact() const;

  double minMaxROC(double, double, double&, double&) const;

  double bearingRateOSCN(double osh, double osv);
  double bearingRateOSCN(double osh, double osv, double time=1);
  double bearingRateCNOS(double osh, double osv, double time=1);
  
  double getcnLAT() const {return(cnLAT);}
  double getcnLON() const {return(cnLON);}
  double getcnCRS() const {return(cnCRS);}
  double getcnSPD() const {return(cnSPD);}
  double getK0() const    {return(statK0);}

  double cnSpdToOS() const {return(m_stat_cn_to_os_spd);}

  unsigned long int getCounter() {return(m_counter);}
  
 protected:
  void   setStatic();
  double smallAngle(double, double) const;

  void   initTrigCache();
  void   initRateCache();

 protected: // Config parameters
  double osLAT;    // Ownship Lat position at time Tm.
  double osLON;    // Ownship Lon position at time Tm.
  double cnLAT;    // Contact Lat position at time Tm.
  double cnLON;    // Contact Lon position at time Tm.
  double cnSPD;    // Contact Speed in kts.
  double cnCRS;    // Contact Course in degrees (0-359).
  double cnCRSx10; // Contact Course in degrees (0-359).

 protected: // Cached values
  double statK2;  // Components of k2, k1, k0 that are 
  double statK1;  // static (independent of the values of
  double statK0;  // osCRS, osSPD, osTOL).
  double statRange;
  
  double statCLOW;  // Course range in which OS is able 
  double statCHGH;  // to cross the path of the contact.
  double statCRNG;  // Range between CLOW and CHGH;
  double statCNANG; // Angle from ownship to the contact.
  double statCNDIS; // Distance from ownship to the contact.

  double stat_cgamCNxcnSPD;
  double stat_sgamCNxcnSPD;
  
  bool   stat_os_on_contact;   // true if ownship is on the contact position
  bool   stat_os_on_bowline;   // true if ownship is on the contact bowline
  bool   stat_os_on_sternline; // true if ownship is on the contact sternline
  double stat_cnx1;
  double stat_cny1;
  double stat_cnx2;
  double stat_cny2;

  double gamCN;   // cnCRS in radians. 
  double cgamCN;  // Cosine of  cnCRS.
  double sgamCN;  // Sine  of   cnCRS.

  double m_stat_cn_to_os_spd;
  bool   m_stat_cn_to_os_closing;
  
  
  std::vector<double> m_cos_cache_3600;
  std::vector<double> m_sin_cache_3600;

  std::vector<double> m_os_vthresh_cache_360;
  
  std::vector<double> m_cos_cache;
  std::vector<double> m_sin_cache;
  std::vector<double> m_cos_sq_cache;
  std::vector<double> m_sin_sq_cache;

  unsigned long int   m_counter;
};

#endif
