/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: main.cpp                                             */
/*    DATE: May 4th, 2010                                        */
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

#include <iostream>
#include <cstdlib>
#include "VZAIC_GUI.h"
#include "PopulatorVZAIC.h"
#include "MBUtils.h"
#include "IvPFunction.h"

using namespace std;

void help_message();

//--------------------------------------------------------
// Procedure: idleProc

void idleProc(void *)
{
  Fl::flush();
  millipause(10);
}

//--------------------------------------------------------
// Procedure: main

int main(int argc, char *argv[])
{
  ZAIC_Vector *zaic = 0;
  int domain = 0;

  string filename;
  for(int i=0; i<argc; i++) {
    string argi = argv[i];
    
    if((argi=="-h") || (argi=="--help")) {
      help_message();
      return(0);
    }     
    else if(strBegins(argi, "--file="))
      filename = tolower(argi.substr(7));
    else if(strEnds(argi, ".zaic"))
      filename = argi;
  }

  if(filename != "") {
    cout << "Filename = " << filename << endl;
    PopulatorVZAIC populator;
    populator.readFile(filename);
    zaic = populator.buildZAIC();
  }


  if(!zaic) {
    cout << "Null initial ZAIC - exiting." << endl;
    exit(0);
  }

  domain = zaic->size();

  Fl::add_idle(idleProc);
  VZAIC_GUI* gui = new VZAIC_GUI(domain+150, 450, "VZAIC-Viewer");
  gui->setZAIC(zaic);

  gui->updateOutput();

  // Enter the GUI event loop.
  return Fl::run();
}


//--------------------------------------------------------
// Procedure: help_message()

void help_message()
{
  cout << endl;
  cout << "Usage: vzaicview                                  " << endl;
  cout << " Parameter     Defaults Description               " << endl;
  cout << " ----------    -------- ------------------------  " << endl;
  cout << " [-help]                Display this help message " << endl;
  cout << " [-d]          410      Set upper value of domain " << endl;
  cout << " [--domain]    410      Set upper value of domain " << endl;
  cout << " [--file]      n/a      Read parameters from file " << endl;
  cout << endl;
}





