/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_SearchGrid.h                                     */
/*    DATE: Sep 3rd, 2005                                        */
/*****************************************************************/
 
#ifndef BHV_SEARCH_GRID_HEADER
#define BHV_SEARCH_GRID_HEADER

#include "IvPBehavior.h"
#include "XYGrid.h"
#include "Timutif.h"

class BHV_SearchGrid : public IvPBehavior {
public:
  BHV_SearchGrid(IvPDomain);
  ~BHV_SearchGrid() {};
  
  IvPFunction* onRunState();
  bool         setParam(std::string, std::string);

protected:
  bool updateFromInfoBuffer();
  bool updateSearchGrid(double, double, double, double);

protected:
  XYGrid    search_grid;
  Timutif   timutif;
  double    time_horizon;

  double    osCRS;
  double    osSPD;
  double    osX;
  double    osY;
  
  double    prev_osX;
  double    prev_osY;

  bool      has_fired;
};
#endif







