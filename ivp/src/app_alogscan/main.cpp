/*****************************************************************/
/*    NAME: Michael Benjamin                                     */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: main.cpp                                             */
/*    DATE: June 3rd, 2008                                       */
/*                                                               */
/* This is unreleased BETA code. No permission is granted or     */
/* implied to use, copy, modify, and distribute this software    */
/* except by the author(s).                                      */
/*****************************************************************/

#include <string>
#include <cstdlib>
#include <iostream>
#include "MBUtils.h"
#include "ScanHandler.h"

using namespace std;

//--------------------------------------------------------
// Procedure: main

int main(int argc, char *argv[])
{
 // Look for a request for version information
  if(scanArgs(argc, argv, "-v", "--version", "-version")) {
    vector<string> svector = getReleaseInfo("alogscan");
    for(unsigned int j=0; j<svector.size(); j++)
      cout << svector[j] << endl;    
    return(0);
  }
  
  // Look for a request for usage information
  if(scanArgs(argc, argv, "-h", "--help", "-help")) {
    cout << "Usage:                                             " << endl;
    cout << "  alogscan file.alog [OPTIONS]                     " << endl;
    cout << "                                                   " << endl;
    cout << "Synopsis:                                          " << endl;
    cout << "  Generate a report on the contents of a given     " << endl;
    cout << "  MOOS .alog file.                                 " << endl;
    cout << "                                                   " << endl;
    cout << "Options:                                           " << endl;
    cout << "  -c,--chars    Sort by total characters (default) " << endl;
    cout << "  -l,--lines    Sort by total lines descending     " << endl;
    cout << "  --start       Sort by start time descending      " << endl;
    cout << "  --stop        Sort by stop time descending       " << endl;
    cout << "  --appstat     Output application statistics      " << endl;
    cout << "  -r,--reverse  Reverse the sorting output         " << endl;
    cout << "  -h,--help     Displays this help message         " << endl;
    cout << "  -v,--version  Displays the current release version" << endl;
    cout << "                                                    " << endl;
    cout << "See also: aloggrp, alogrm, alogclip, alogview       " << endl;
    return(0);
  }

  bool   reverse_requested  = false;
  bool   app_stat_requested = false;
  string sort_style = "bychars_ascending";

  string alogfile = "";
  for(int i=1; i<argc; i++) {
    string sarg = tolower(argv[i]);
    if(strContains(sarg, ".alog"))
      alogfile = sarg;
    else if((sarg == "-c") || (sarg == "--chars"))
      sort_style = "bychars_ascending";
    else if((sarg == "-l") || (sarg == "--lines"))
      sort_style = "bylines_ascending";
    else if(sarg == "--start")
      sort_style = "bystarttime_ascending";
    else if(sarg, "--stop")
      sort_style = "bystoptime_ascending";
    else if(sarg == "--appstat")
      app_stat_requested = true;
    else if((sarg == "-r") || (sarg == "--reversed"))
      reverse_requested = true;
  }
 
  if(reverse_requested) {
    if(sort_style == "bychars_ascending")
      sort_style = "bychars_descending";
    else if(sort_style == "bylines_ascending")
      sort_style = "bylines_descending";
    else if(sort_style == "bystart_ascending")
      sort_style = "bystart_descending";
    else if(sort_style == "bystop_ascending")
      sort_style = "bystop_descending";
  }

  cout << "Processing on file : " << alogfile << endl;

  ScanHandler handler;
  handler.setParam("sort_style", sort_style);
  handler.handle(alogfile);
  
  if(app_stat_requested)
    handler.appStatReport();

}






