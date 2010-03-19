/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: HelmIvP.cpp                                          */
/*    DATE: Oct 12th 2004                                        */
/*****************************************************************/

#include <iterator>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "HelmIvP.h"
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "IvPProblem.h"
#include "HelmReport.h"
#include "stringutil.h"
#include "Populator_BehaviorSet.h"

using namespace std;

//--------------------------------------------------------------------
// Procedure: Constructor

HelmIvP::HelmIvP()
{
  m_has_control    = false;
  m_bhv_set        = 0;
  m_hengine        = 0;
  m_info_buffer    = 0;
  m_iteration      = 0;
  m_curr_time_updated = false;

  m_node_report_vars.push_back("NODE_REPORT");
  m_node_report_vars.push_back("NODE_REPORT_LOCAL");
}

//--------------------------------------------------------------------
// Procedure: OnNewMail()

bool HelmIvP::OnNewMail(MOOSMSG_LIST &NewMail)
{
  // The curr_time is set in *both* the OnNewMail and Iterate functions.
  // In the OnNewMail function so the most up-to-date time is available
  // when processing mail.
  // In the Iterate method to ensure behaviors are not iterated with an
  // un-initialized timestamp on startup, and in case there is no Mail 
  m_curr_time = MOOSTime();
  m_info_buffer->setCurrTime(m_curr_time);
  m_curr_time_updated = true;

  MOOSMSG_LIST::iterator p;
  
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    if((msg.m_sKey == "MOOS_MANUAL_OVERIDE") || 
       (msg.m_sKey == "MOOS_MANUAL_OVERRIDE")) {
      if(MOOSStrCmp(msg.m_sVal, "FALSE"))
	m_has_control = true;
    }
    else if(vectorContains(m_node_report_vars, msg.m_sKey))
      processNodeReport(msg.m_sVal);
    else
      updateInfoBuffer(msg);
  }
  
  return(true);
}

//--------------------------------------------------------------------
// Procedure: Iterate()
//      Note: happens AppTick times per second
//            

bool HelmIvP::Iterate()
{
  // If the curr_time is not set in the OnNewMail function (possibly 
  // because there was no mail in the queue), set the current time now.
  if(!m_curr_time_updated) {
    m_curr_time = MOOSTime();
    m_info_buffer->setCurrTime(m_curr_time);
  }

  // Now we're done addressing whether the curr_time is updated on this
  // iteration. It was done either in this function or in onNewMail().
  // Now set m_curr_time_updated=false to reflect the ingoing state for
  // the next iteration.
  m_curr_time_updated = false;

  if(!m_has_control)
    return(false);

  HelmReport helm_report;
  helm_report = m_hengine->determineNextDecision(m_bhv_set, m_curr_time);

  m_iteration = helm_report.getIteration();
 

  postBehaviorMessages();

  m_Comms.Notify("HELM_IPF_COUNT", helm_report.getOFNUM());
  m_Comms.Notify("IVPHELM_SUMMARY", helm_report.getReportAsString());
 
  if(helm_report.getHalted()) {
    m_has_control = false;
    bool ok;
    string bhv_error_str = m_info_buffer->sQuery("BHV_ERROR", ok);
    if(!ok)
      bhv_error_str = " - unknown - ";
    MOOSDebugWrite("BHV_ERROR: " + bhv_error_str);
    MOOSDebugWrite("pHelmIvP Control Is Off: All Dec-Vars set to ZERO");
  }
  
  int dsize = m_ivp_domain.size();

  // First make sure the HelmEngine has made a decision for all 
  // non-optional variables - otherwise declare an incomplete decision.
  // If a complete decision is not generated, this does not mean the 
  // helm relinquishes control, only that an all-stop is posted on 
  // this iteration.
  bool complete_decision = false;
  if(m_has_control && helm_report.getOFNUM() > 0) {
    complete_decision = true;
    for(int i=0; i<dsize; i++) {
      string domain_var = m_ivp_domain.getVarName(i);
      if(!helm_report.hasDecision(domain_var)) {
	complete_decision = false;
	string s1 = "ERROR! No decision for mandatory var - " + domain_var;
	string s2 = "pHelmIvP Control is Off: All Dec-Vars set to ZERO";
	MOOSDebugWrite(s1);
	MOOSDebugWrite(s2);
      }
    }
  }
  
  if(m_has_control && complete_decision) {
    for(int j=0; j<dsize; j++) {
      string domain_var = m_ivp_domain.getVarName(j);
      string post_alias = "DESIRED_"+ toupper(domain_var);
      if(post_alias == "DESIRED_COURSE")
	post_alias = "DESIRED_HEADING";
      double domain_val = helm_report.getDecision(domain_var);
      m_Comms.Notify(post_alias, domain_val);
    }
  }
  
  double create_cpu  = helm_report.getCreateTime();
  m_Comms.Notify("CREATE_CPU", create_cpu);

  double loop_cpu  = helm_report.getLoopTime();
  m_Comms.Notify("LOOP_CPU", loop_cpu);

  // Clear the delta vectors now that all behavior have had the 
  // chance to consume delta info.
  m_info_buffer->clearDeltaVectors();

  return(true);
}
  

//------------------------------------------------------------
// Procedure: postBehaviorMessages()
//      Note: Run once after every iteration of control loop.
//            Each behavior has the chance to produce their 
//            own message to posted in both the info_buffer
//            and in the MOOSDB. 
//            By posting to the MOOSDB and using a mechanism
//            for sharing vars between communities, i.e, 
//            pMOOSBridge, or MOOSLink, inter-vehicle coordin.
//            can be accomplished.

void HelmIvP::postBehaviorMessages()
{
  if(!m_bhv_set) 
    return;
  
  unsigned int i, bhv_cnt = m_bhv_set->size();
  m_Comms.Notify("BCOUNT", bhv_cnt);
  m_Comms.Notify("TBCOUNT", m_bhv_set->getTCount());
  m_Comms.Notify("HITER", m_iteration);
  for(i=0; i < bhv_cnt; i++) {
    string bhv_descriptor = m_bhv_set->getDescriptor(i);
    vector<VarDataPair> mvector = m_bhv_set->getMessages(i);
    unsigned int msize = mvector.size();

    string bhv_postings_summary;
  
    //for(int j=0; j<msize; j++) {
    for(int j=0; (j<2)&&(j<msize); j++) {
      VarDataPair msg = mvector[j];

      string var   = msg.get_var();
      string sdata = msg.get_sdata();
      double ddata = msg.get_ddata();
      string mkey  = msg.get_key();
      bool   msg_is_string = msg.is_string();

      bool key_change = true;
      bool key_repeat = true;

      if(msg_is_string) {
	m_Comms.Notify("LEN", sdata.length());
	m_Comms.Notify(var, sdata);
      }
      else
	m_Comms.Notify(var, ddata);
    }
    //mikerb
    //    if(bhv_postings_summary != "")
    //  m_Comms.Notify("IVPHELM_POSTINGS", bhv_postings_summary, bhv_descriptor);
  }
  // Determine if the list of state-space related variables for
  // the behavior-set has changed and post the new set if so.

  return;  //mikerb
  bool changed = m_bhv_set->updateStateSpaceVars();
  if(changed) {
    string state_vars = m_bhv_set->getStateSpaceVars();
    m_Comms.Notify("IVPHELM_STATEVARS", state_vars);
  }
}

//------------------------------------------------------------
// Procedure: postIntialVariables()

void HelmIvP::postInitialVariables()
{
  if(!m_bhv_set) return;

  vector<VarDataPair> mvector = m_bhv_set->getInitialVariables();
  int msize = mvector.size();
  for(int j=0; j<msize; j++) {
    VarDataPair msg = mvector[j];
    
    string var   = stripBlankEnds(msg.get_var());
    string sdata = stripBlankEnds(msg.get_sdata());
    double ddata = msg.get_ddata();
    
    if(sdata != "") {
      m_info_buffer->setValue(var, sdata);
      m_Comms.Notify(var, sdata);
    }
    else {
      m_info_buffer->setValue(var, ddata);
      m_Comms.Notify(var, ddata);
    }
  }
}

//------------------------------------------------------------
// Procedure: updateInfoBuffer()

bool HelmIvP::updateInfoBuffer(CMOOSMsg &msg)
{
  string key = msg.m_sKey;
  string sdata = msg.m_sVal;
  double ddata = msg.m_dfVal;
  char   mtype = msg.m_cDataType;

  if(mtype == MOOS_DOUBLE) 
    return(m_info_buffer->setValue(key, ddata));
  else if(mtype == MOOS_STRING) 
    return(m_info_buffer->setValue(key, sdata));
  else
    return(false);
}

//------------------------------------------------------------
// Procedure: OnConnectToServer()

bool HelmIvP::OnConnectToServer()
{
  registerVariables();
  return(true);
}

//------------------------------------------------------------
// Procedure: registerVariables

void HelmIvP::registerVariables()
{
  m_Comms.Register("MOOS_MANUAL_OVERIDE", 0);
  m_Comms.Register("MOOS_MANUAL_OVERRIDE", 0);

  // Register for node report variables, e.g., AIS_REPORT, NODE_REPORT
  unsigned int i, vsize = m_node_report_vars.size();
  for(i=0; i<vsize; i++) 
    m_Comms.Register(m_node_report_vars[i], 0);
  
  if(m_bhv_set) {
    vector<string> info_vars = m_bhv_set->getInfoVars();
    for(unsigned int j=0; j<info_vars.size(); j++)
      m_Comms.Register(info_vars[j], 0.0);
  }
}

//--------------------------------------------------------
// Procedure: onStartUp()

bool HelmIvP::OnStartUp()
{
  if(!m_info_buffer)
    m_info_buffer = new InfoBuffer;

  // Look for ownship name first - a "global" variable in the 
  // Configuration file. 
  m_ownship = "unknown";
  if(!m_MissionReader.GetValue("Community", m_ownship))
    return(false);
  
  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  m_MissionReader.GetConfiguration(GetAppName(), sParams);
    
  STRING_LIST::iterator p;
  for(p = sParams.begin();p!=sParams.end();p++) {
    string line  = *p;
    string param = stripBlankEnds(toupper(biteString(line, '=')));
    string value = stripBlankEnds(line);

    if(param == "BEHAVIORS")
      addBehaviorFile(value);
    else if(param == "DOMAIN")
      handleDomainEntry(value);
  }
    
  m_hengine = new HelmEngineBeta(m_ivp_domain, m_info_buffer);

  Populator_BehaviorSet *p_bset;
  p_bset = new Populator_BehaviorSet(m_ivp_domain, m_info_buffer);
  p_bset->loadEnvVarDirectories("IVP_BEHAVIOR_DIRS", true);
  m_bhv_set = p_bset->populate(m_bhv_files);

  if(m_bhv_set == 0)
    return(false);

  // Set the "ownship" parameter for all behaviors
  for(int i=0; i<m_bhv_set->getCount(); i++) {
    m_bhv_set->getBehavior(i)->IvPBehavior::setParam("us", m_ownship);
    m_bhv_set->getBehavior(i)->onSetParamComplete();
  }
  
  postInitialVariables();
  registerVariables();
  return(true);
}

//--------------------------------------------------------------------
// Procedure: addBehaviorFile

void HelmIvP::addBehaviorFile(string filename)
{
  m_bhv_files.insert(filename);
}

//--------------------------------------------------------------------
// Procedure: handleDomainEntry

bool HelmIvP::handleDomainEntry(const string& g_entry)
{
  string entry = stripBlankEnds(g_entry);
  entry = findReplace(entry, ',', ':');

  vector<string> svector = parseString(entry, ':');
  int vsize = svector.size();
  if((vsize < 4) || (vsize > 5))
    return(false);

  string dname = svector[0];
  double dlow  = atof(svector[1].c_str());
  double dhgh  = atof(svector[2].c_str());
  int    dcnt  = atoi(svector[3].c_str());
  
  bool ok = m_ivp_domain.addDomain(dname.c_str(), dlow, dhgh, dcnt);
  return(ok);
}


//--------------------------------------------------------------------
// Procedure: processNodeReport
//   Purpose: 

bool HelmIvP::processNodeReport(const string& report)
{
  string x_val, dep_val, spd_val, hdg_val, vname;
  string y_val, utc_val, lat_val, long_val;
  
  vector<string> svector = parseString(report, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string left  = stripBlankEnds(biteString(svector[i], '='));
    string right = stripBlankEnds(svector[i]);
    bool right_isnum = isNumber(right);

    if(left=="NAME")
      vname = toupper(right);
    else if((left == "UTC_TIME") && right_isnum)
      utc_val = right;
    else if((left == "MOOS_TIME") && right_isnum) 
      utc_val = right;
    else if((left == "X") && right_isnum)
      x_val = right;
    else if((left == "Y") && right_isnum)
      y_val = right;
    else if((left == "LAT") && right_isnum)
      lat_val = right;
    else if((left == "LON") && right_isnum)
      long_val = right;
    else if((left == "SPD") && right_isnum)
      spd_val = right;
    else if((left == "HDG") && right_isnum)
      hdg_val = right;
    else if((left == "DEPTH") && right_isnum) 
      dep_val = right;
  }

  if((x_val=="") || (spd_val=="") || (utc_val=="") || (vname=="") ||
     (y_val=="") || (hdg_val=="") || (dep_val==""))
    return(false);

  m_info_buffer->setValue(vname+"_NAV_UTC", atof(utc_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_X", atof(x_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_Y", atof(y_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_SPEED", atof(spd_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_HEADING", atof(hdg_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_DEPTH", atof(dep_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_LAT", atof(lat_val.c_str()));
  m_info_buffer->setValue(vname+"_NAV_LONG", atof(long_val.c_str()));
  
  return(true);
}

