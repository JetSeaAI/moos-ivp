/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: NodeBroker.cpp                                       */
/*    DATE: Dec 19th 2011                                        */
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

#include <iterator>
#include "HostRecord.h"
#include "HostRecordUtils.h"
#include "UFieldUtils.h"
#include "NodeBroker.h"
#include "ACBlock.h"
#include "MBUtils.h"
#include "ACTable.h"

using namespace std;

//---------------------------------------------------------
// Constructor

NodeBroker::NodeBroker()
{ 
  // Initialize State Variables
  m_pshare_cmd_posted = 0;
  m_pings_posted      = 0;
  m_ok_phis_received  = 0;
  m_bad_phis_received = 0;
  m_ok_acks_received  = 0;
  m_bad_acks_received = 0;
  m_host_info_changes = 0;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool NodeBroker::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 
    string comm  = msg.GetCommunity();

    bool msg_is_local = true;
    if(msg.GetCommunity() != m_node_host_record.getCommunity())
      msg_is_local = false;

    //double dval  = msg.GetDouble();
    //double mtime = msg.GetTime();
    //bool   mdbl  = msg.IsDouble();
    //bool   mstr  = msg.IsString();
    //string msrc  = msg.GetSource();
    
    // Only accept HOST_INFO coming from our own community
    if((key == "PHI_HOST_INFO") && msg_is_local) 
      handleMailHostInfo(sval);

    // Only accept an ACK coming from a different community
    else if((key == "NODE_BROKER_ACK") && !msg_is_local) 
      handleMailAck(sval); 
    
    else 
      reportRunWarning("Unhandled Mail: " + key);    
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool NodeBroker::OnConnectToServer()
{
  registerVariables();  
  return(true);
}


//---------------------------------------------------------
// Procedure: Iterate()

bool NodeBroker::Iterate()
{
  AppCastingMOOSApp::Iterate();

  sendNodeBrokerPing();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()

bool NodeBroker::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  // host_community info grabbed and stored at AppCastingMOOSApp level
  m_node_host_record.setCommunity(m_host_community);
  
  STRING_LIST sParams;
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams)) 
    reportConfigWarning("No config block found for " + GetAppName());
  
  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "TRY_SHORE_HOST") 
      handled = handleConfigTryShoreHost(value);
    else if(param == "BRIDGE") 
      handled = handleConfigBridge(value);

    if(!handled)
      reportUnhandledConfigWarning(orig);
  }

  registerVariables();
  registerPingBridges();
  return(true);
}

//------------------------------------------------------------
// Procedure: registerVariables

void NodeBroker::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();

  m_Comms.Register("NODE_BROKER_ACK", 0);
  m_Comms.Register("PHI_HOST_INFO", 0);
}


//------------------------------------------------------------
// Procedure: sendNodeBrokerPing()

void NodeBroker::sendNodeBrokerPing()
{
  // If we know enough about ourself, then ping away!!
  if((m_node_host_record.getPShareIRoutes() != "") &&
     (m_node_host_record.getTimeWarp() != "")) {
    
    string iroutes = m_node_host_record.getPShareIRoutes();
    string ipaddr  = m_node_host_record.getHostIP();
    iroutes = findReplace(iroutes, "localhost", ipaddr);

    string tstamp = doubleToString(m_curr_time, 2);
    m_node_host_record.setTimeStamp(tstamp);
    Notify("NODE_BROKER_PING", m_node_host_record.getSpec());
    m_pings_posted++;
  }
}


//------------------------------------------------------------
// Procedure: registerPingBridges

void NodeBroker::registerPingBridges()
{
  unsigned int i, vsize = m_shore_routes.size();
  for(i=0; i<vsize; i++) {
    string src  = "NODE_BROKER_PING";
    string dest = "NODE_BROKER_PING";
    string route = m_shore_routes[i];
    postPShareCommand(src, dest, route);
  }
}

//------------------------------------------------------------
// Procedure: registerUserBridges

void NodeBroker::registerUserBridges()
{
  // Check each candidate shore node
  unsigned int k, ksize = m_shore_routes.size();
  for(k=0; k<ksize; k++) {
    // Check if the shore candidate (a) has been heard from with
    // a valid iroute, and (b) not yet had user variables bridged
    if(!m_shore_bridged[k] && (m_shore_pings_ack[k] > 0)) {
      m_shore_bridged[k] = true;

      unsigned int i, vsize = m_bridge_src_var.size();
      for(i=0; i<vsize; i++) {			  
	string src   = m_bridge_src_var[i];
	string dest  = m_bridge_alias[i];
	string route = m_shore_routes[k];
	postPShareCommand(src, dest, route);
      }
    }
  }
}


//------------------------------------------------------------
// Procedure: handleConfigBridge
//   Example: BRIDGE = src=FOO, alias=BAR

bool NodeBroker::handleConfigBridge(string line)
{
  string src, alias;

  vector<string> svector = parseString(line, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string left  = tolower(biteStringX(svector[i],'='));
    string right = svector[i];
    if(left == "src")
      src = right;
    else if(left == "alias")
      alias = right;
  }

  if((src == "") || strContainsWhite(src))
    return(false);

  if((alias == "") || strContainsWhite(alias))
    alias = src;
  
  m_bridge_src_var.push_back(src);
  m_bridge_alias.push_back(alias);
  return(true);
}


//------------------------------------------------------------
// Procedure: handleConfigTryShoreHost

bool NodeBroker::handleConfigTryShoreHost(string original_line)
{
  string pshare_route;

  vector<string> svector = parseString(original_line, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string left  = biteStringX(svector[i], '=');
    string right = svector[i];
    if(left == "pshare_route")
      pshare_route = right;
    else 
      return(false);
  }
  
  if(!isValidPShareRoute(pshare_route))
    return(false);
  
  m_shore_routes.push_back(pshare_route);
  m_shore_community.push_back("");
  m_shore_pings_ack.push_back(0);
  m_shore_ipaddr.push_back("");
  m_shore_timewarp.push_back("");
  m_shore_bridged.push_back(false);
  return(true);
}

//------------------------------------------------------------
// Procedure: handleMailAck
//      Note: Parse NODE_BROKER_ACK and update the info about the
//            shoreside node that sent it.

void NodeBroker::handleMailAck(string ack_msg)
{
  // Part 1: Build/validate the incoming Host Record
  HostRecord hrecord = string2HostRecord(ack_msg);

  string pshare_iroutes = hrecord.getPShareIRoutes();
  if(pshare_iroutes == "") {
    m_bad_acks_received++;

    string msg = "NODE_BROKER_ACK recvd from " + hrecord.getHostIP();
    msg += " w/ null pshare_iroutes, for now.";

    reportRunWarning(msg);
    return;
  }

  // Part 3: If the remote hostip != the local hostip, find/replace 
  // "localhost" with remote hostip if localhost appears in the iroute.

  string hostip_shore = hrecord.getHostIP();
  string hostip_local = m_node_host_record.getHostIP();
  if(hostip_shore != hostip_local) 
    pshare_iroutes = findReplace(pshare_iroutes, "localhost", hostip_shore);

  // Note the sender may be reporting more then one input route
  vector<string> svector = parseString(pshare_iroutes, '&');
  unsigned int i, vsize = svector.size();

  for(i=0; i<vsize; i++) {
    string iroute = stripBlankEnds(svector[i]);
    // Now see if the incoming iroute matches one of the try_hosts
    unsigned int j, jsize = m_shore_routes.size();
    for(j=0; j<jsize; j++) {

      // It's possible that the iroute received from the shore ack may read
      // something like "10.0.0.7:9300" where the IP is the localhost IP.
      // And tryhost read "localhost:9300". In this case we want the match
      // to succeed. So we create a "modified shore route", mod_sroute below
      // which will convert "localhost:9300" to "10.0.0.7.9300" and test 
      // against that also
      string mod_sroute = findReplace(m_shore_routes[j], "localhost", 
				      m_node_host_record.getHostIP());

      if((iroute == m_shore_routes[j]) ||
	 (iroute == mod_sroute)) {
	m_shore_community[j] = hrecord.getCommunity();
	m_shore_pings_ack[j]++;
	m_shore_ipaddr[j]    = hrecord.getHostIP();
	m_shore_timewarp[j]  = hrecord.getTimeWarp();
	m_ok_acks_received++;

	string msg = "NODE_BROKER_ACK recvd from " + hrecord.getHostIP();
	msg += " w/ null pshare_iroutes, for now.";
	retractRunWarning(msg);
      }
    }
  }

  // Set up the user-configured variable bridges.
  registerUserBridges();
}


//------------------------------------------------------------
// Procedure: handleMailHostInfo

void NodeBroker::handleMailHostInfo(string phi_msg)
{
  HostRecord hrecord = string2HostRecord(phi_msg);
  if(hrecord.valid() == false) {
    m_bad_phis_received++;
    reportRunWarning("Invalid PHI_HOST_INFO recd: " + phi_msg);
    return;
  }
  m_ok_phis_received++;
  
  // Set up the localhost bridges if the host info has changed.
  if((m_node_host_record.getCommunity()  != hrecord.getCommunity()) ||
     (m_node_host_record.getTimeWarp()   != hrecord.getTimeWarp())  ||
     (m_node_host_record.getHostIP()     != hrecord.getHostIP())    ||
     (m_node_host_record.getPortDB()     != hrecord.getPortDB())    ||
     (m_node_host_record.getPortUDP()    != hrecord.getPortUDP())   ||
     (m_node_host_record.getPShareIRoutes() != hrecord.getPShareIRoutes())) {
    m_host_info_changes++;
  }

  m_node_host_record = hrecord;
}

//------------------------------------------------------------
// Procedure: postPShareCommand

void NodeBroker::postPShareCommand(string src, string dest, string route)
{
  string msg = "cmd=output";
  msg += ",src_name="  + src;
  msg += ",dest_name=" + dest;
  msg += ",route=" + route;
  Notify("PSHARE_CMD", msg);
  
  m_pshare_cmd_posted++;
  m_pshare_cmd_latest = msg;
}
  
//------------------------------------------------------------
// Procedure: buildReport
//      Note: A virtual function of the AppCastingMOOSApp superclass, 
//            conditionally invoked if either a terminal or appcast 
//            report is needed.
//
bool NodeBroker::buildReport()
{
  m_msgs << endl;
  m_msgs << "  Total OK  PHI_HOST_INFO    received: " << m_ok_phis_received  << endl;
  m_msgs << "  Total BAD PHI_HOST_INFO    received: " << m_bad_phis_received << endl;
  m_msgs << "  Total HOST_INFO changes    received: " << m_host_info_changes << endl;
  m_msgs << "  Total          PSHARE_CMD    posted: " << m_pshare_cmd_posted << endl;
  m_msgs << "  Total BAD NODE_BROKER_ACK  received: " << m_bad_acks_received << endl;
  m_msgs << endl;
  m_msgs << "===========================================================" << endl;
  m_msgs << "             Vehicle Node Information:" << endl;
  m_msgs << "===========================================================" << endl;
  m_msgs << endl;
  m_msgs << "     Community: " << m_node_host_record.getCommunity()       << endl; 
  m_msgs << "        HostIP: " << m_node_host_record.getHostIP()          << endl; 
  m_msgs << "   Port MOOSDB: " << m_node_host_record.getPortDB()          << endl; 
  m_msgs << "     Time Warp: " << m_node_host_record.getTimeWarp()        << endl; 

  string iroutes = m_node_host_record.getPShareIRoutes();
  string ipaddr  = m_node_host_record.getHostIP();
  iroutes = findReplace(iroutes, "localhost", ipaddr);
  ACBlock block("      IRoutes: ", iroutes, 1, '&');
  m_msgs << block.getFormattedString();

  m_msgs << endl;
  m_msgs << "===========================================================" << endl;
  m_msgs << "            Shoreside Node(s) Information:" << endl;
  m_msgs << "===========================================================" << endl;
  m_msgs << endl;

  ACTable actab(6);
  actab << "Community |       | Pings | Pings | IP      | Time";
  actab << "Name      | Route | Sent  | Acked | Address | Warp";
  actab.addHeaderLines();
  unsigned int k, ksize = m_shore_routes.size();
  for(k=0; k<ksize; k++) {
    actab << m_shore_community[k];
    actab << m_shore_routes[k];
    actab << m_pings_posted;
    actab << m_shore_pings_ack[k];
    actab << m_shore_ipaddr[k];
    actab << m_shore_timewarp[k];
  }
  m_msgs << actab.getFormattedString();

  m_msgs << endl << endl;

  m_msgs << "Phase Completion Summary:"             << endl;
  m_msgs << "------------------------------------" << endl;
  string s1 = (m_ok_phis_received > 0) ? "(Y)" : "(N)";
  m_msgs << " Phase 1: " << s1 << " Valid Host information retrieved (iroutes)." << endl;
  
  string s2 = (m_shore_routes.size() > 0) ? "(Y)" : "(N)";
  string num2 = uintToString(m_shore_routes.size());
  m_msgs << " Phase 2: " << s2 << " Valid TryHosts (" << num2 << ") configured." << endl;
  
  string s3 = (m_pings_posted > 0) ? "(Y)" : "(N)";
  m_msgs << " Phase 3: " << s3 << " NODE_BROKER_PINGS are being sent to TryHosts." << endl;

  string s4 = (m_ok_acks_received > 0) ? "(Y)" : "(N)";
  m_msgs << " Phase 4: " << s4 << " A Valid NODE_BROKER_ACK has been received." << endl;

  bool user_vars_bridged = false;
  for(unsigned int i=0; i<m_shore_bridged.size(); i++)
    user_vars_bridged = user_vars_bridged || m_shore_bridged[i];

  string s5 = user_vars_bridged ? "(Y)" : "(N)";
  m_msgs << " Phase 5: "<<s5<<" pShare requested to share user vars with shoreside." << endl;

  if(s1 == "(N)")
    m_msgs << "*Hint: Phase 1 problem. Check status/output of pHostInfo.              " << endl;
  else if(s2 == "(N)")
    m_msgs << "*Hint: Phase 2 problem. Check your .moos file configuration.           " << endl;
  else if(s3 == "(N)")
    m_msgs << "*Hint: Phase 3 problem. Check if our node's iroutes, timewarp are set. " << endl;
  else if(s4 == "(N)") {
    m_msgs << "*Hint: Phase 4 problem. Check that shoreside is running and            " << endl;
    m_msgs << "      that shoreside has received this node's NODE_BROKER_PING.        " << endl;
    m_msgs << "      If the shoreside HAS NOT received the ping, check that the       " << endl;
    m_msgs << "      pShare input route matches one of the TryHosts in uFldNodeBroker." << endl; 
    m_msgs << "      If the shoreside HAS received a ping, check that we have received" << endl;
    m_msgs << "      a NODE_BROKER_ACK in the local MOOSDB."                            << endl;
  }    
  else if(s5 == "(N)") {
    m_msgs << "*Hint: Phase 5 problem. Check that the user has indeed configured " << endl;
    m_msgs << "      in the .moos config block for this app."                      << endl;
  }    
  else
    m_msgs << "All Phases complete. Things should be working as configured. "      << endl;

  return(true);
}

