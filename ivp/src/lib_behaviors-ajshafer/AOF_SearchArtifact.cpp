/*****************************************************************/
/*    NAME: Andrew Shafer, Mike Benjamin                         */
/*    ORGN: MIT                                                  */
/*    FILE: AOF_SearchArtifact.cpp                               */
/*    DATE: NOV 28, 2007                                         */
/*****************************************************************/

#ifdef _WIN32
#pragma warning(disable : 4786)
#pragma warning(disable : 4503)
#endif
#include <math.h> 
#include "AOF_SearchArtifact.h"
#include "AngleUtils.h"
#include "MBUtils.h"

using namespace std;

//----------------------------------------------------------
// Procedure: Constructor

AOF_SearchArtifact::AOF_SearchArtifact(IvPDomain g_domain, const XYArtifactGrid *g_grid) : AOF(g_domain)
{
	crs_ix = g_domain.getIndex("course");
	spd_ix = g_domain.getIndex("speed");

	search_grid  = g_grid;
	time_horizon = 60;       // 60 Seconds by default
}

//----------------------------------------------------------------
// Procedure: setParam

bool AOF_SearchArtifact::setParam(const string& param, double param_val)
{
	if(param == "os_lat") {
		os_lat = param_val;
		os_lat_set = true;
		return(true);
	}
	else if(param == "os_lon") {
		os_lon = param_val;
		os_lon_set = true;
		return(true);
	}
	else if(param == "time_horizon") {
		time_horizon = param_val;
		return(true);
	}
	else
		return(false);
}


//----------------------------------------------------------------
// Procedure: setParam

bool AOF_SearchArtifact::setParam(const string& param, 
			      const string& param_val)
{
	if(param == "pass_value") {
		pass_value = SearchPassValue(param_val);
		return(true);
	}
	else
		return(false);
}


//----------------------------------------------------------------
// Procedure: initialize

bool AOF_SearchArtifact::initialize()
{
	if( (crs_ix==-1) || (spd_ix==-1) )
		return(false);

	if( !os_lat_set ||  !os_lon_set )
		return(false);

	return(true);
}


//----------------------------------------------------------------
// Procedure: fillCache

void AOF_SearchArtifact::fillCache()
{
	ray_cache.clear();

	int i;
	double heading;
	double top_spd    = m_domain.getVarHigh(spd_ix);
	int heading_count = m_domain.getVarPoints(crs_ix);

	// For each possible heading...
	for(i=0; i<heading_count; i++) {
		vector<int> ivector;
		ray_cache.push_back(ivector);
	}

	// ...calculate the intercepted cells going that direction
	for(i=0; i<heading_count; i++) {
		bool ok = m_domain.getVal(crs_ix, i, heading);

		if(!ok) { // Something's wrong...
			ray_cache.clear();
			return;
		}

		double gamOS  = degToRadians(heading); // Angle in radians.
		double cgamOS = cos(gamOS);            // Cosine of Angle (osCRS).
		double sgamOS = sin(gamOS);            // Sine   of Angle (osCRS).

		double eval_lat = (cgamOS * top_spd * time_horizon) + os_lat;
		double eval_lon = (sgamOS * top_spd * time_horizon) + os_lon;

		double x1 = os_lon;
		double y1 = os_lat;
		double x2 = eval_lon;
		double y2 = eval_lat;

		// Each vector in ray_cache is a heading
		// Each int in the heading vector is the index of a square in the
		// search grid that is intercepted.
		int gsize = search_grid->size();
		for(int j=0; j<gsize; j++) {
			XYSquare square = search_grid->getElement(j);
			if(!square.containsPoint(x1,y1)) { // Don't count the current location
				double length = square.segIntersectLength(x1,y1,x2,y2);
				if(length > 0){
					ray_cache[i].push_back(j);
				}
			}
		}
	}
}

//----------------------------------------------------------------
// Procedure: evalBox
//   Purpose: Evaluates a given <Course, Speed, Time-on-leg> tuple 
//               given by a 3D ptBox (b).
//            Determines naut mile Closest-Point-of-Approach (CPA)
//               and returns a value after passing it thru the 
//               metric() function.

double AOF_SearchArtifact::evalBox(const IvPBox *b) const
{
	double eval_crs, eval_spd;

	double eval_tol = time_horizon;

	m_domain.getVal(crs_ix, b->pt(crs_ix,0), eval_crs);
	m_domain.getVal(spd_ix, b->pt(spd_ix,0), eval_spd);

	double gamOS  = degToRadians(eval_crs); // Angle in radians.
	double cgamOS = cos(gamOS);             // Cosine of Angle (osCRS).
	double sgamOS = sin(gamOS);             // Sine   of Angle (osCRS).

	double eval_lat = (cgamOS * eval_spd * eval_tol) + os_lat;
	double eval_lon = (sgamOS * eval_spd * eval_tol) + os_lon;

	double x1 = os_lon;
	double y1 = os_lat;
	double x2 = eval_lon;
	double y2 = eval_lat;

	double ret_val = 0;

	// Calculate the course index, crs_index, which is the integer
	// counterpart in the domain. 
	// For example if the domain is 0:359:360 then
	//    359 --> 359
	//    15  -->  15
	// For example if the domain is 0:359:180 then
	//    359 -->  89
	//    15  -->   7
	// For example if the domain is 0:359:720 then
	//    359 -->  718
	//    15  -->   30

	double crs_delta = m_domain.getVarDelta(crs_ix);
	int    cache_i   = (int)(eval_crs / crs_delta);
	int    vsize     = ray_cache[cache_i].size(); 

	double top_spd   = m_domain.getVarHigh(spd_ix);
	double top_dist  = top_spd * time_horizon;

	// For each intersected cell, calculate the value of that pass
	for(int cache_j=0; cache_j < vsize; cache_j++) {
		int grid_ix = ray_cache[cache_i][cache_j];
		XYSquare square = search_grid->getElement(grid_ix);
		double length = square.segIntersectLength(x1,y1,x2,y2);
		if((eval_spd > 0) && (length > 0)) {
			double dist_to_square = square.ptDistToSquareCtr(x1,y1);
			double time_in_square = length / eval_spd;
			double curr_duration  = search_grid->getVal(grid_ix);
			double curr_util      = pass_value.evalValue(curr_duration);
			double hypo_util      = pass_value.evalValue(curr_duration + time_in_square);
			double delta_util     = hypo_util - curr_util;
			
			// Why is this done 6 times?
			// Each line appears to multiply the delta utility by the portion of time
			// that the cell will take to get that extra utility
			// i.e., a cell update that takes 1/4 the top_dist and gives 4 delta_util
			// is more valuable than an update that takes 1/4 the top_dist and gives 6 delta_util
			delta_util *= (1 - (dist_to_square / top_dist));
			delta_util *= (1 - (dist_to_square / top_dist));
			delta_util *= (1 - (dist_to_square / top_dist));
			delta_util *= (1 - (dist_to_square / top_dist));
			delta_util *= (1 - (dist_to_square / top_dist));
			delta_util *= (1 - (dist_to_square / top_dist));

			ret_val += delta_util;
		}
	}

  return(ret_val);
}

