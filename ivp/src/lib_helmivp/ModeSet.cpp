/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: ModeSet.cpp                                          */
/*    DATE: Dec 26th, 2008                                       */
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
#include "ModeSet.h"
#include "MBUtils.h"

using namespace std;

//------------------------------------------------------------------
// Procedure: addEntry

bool ModeSet::addEntry(const string& str)
{
  ModeEntry new_entry;
  bool ok = new_entry.setEntry(str);
  if(ok)
    m_entries.push_back(new_entry);
  return(ok);
}

//------------------------------------------------------------------
// Procedure: consultFromInfoBuffer
//      Note: Get values of all variables from the info_buffer and 
//            propogate these values down to all the logic conditions.

void ModeSet::consultFromInfoBuffer()
{
  unsigned int j, esize = m_entries.size();

  vector<string> all_vars = getConditionVars();
  unsigned int i, vsize = all_vars.size();
  for(i=0; i<vsize; i++) {
    string varname = all_vars[i];
    bool   ok_s, ok_d;
    string s_result = m_info_buffer->sQuery(varname, ok_s);
    double d_result = m_info_buffer->dQuery(varname, ok_d);

    for(j=0; ((j<esize) && (ok_s)); j++)
      m_entries[j].setVarVal(varname, s_result);

    for(j=0; ((j<esize) && (ok_d)); j++)
      m_entries[j].setVarVal(varname, d_result);    
  }

}
//------------------------------------------------------------------
// Procedure: updateInfoBuffer
//      Note: For each of the VarDataPairs stored locally in the member
//            variable m_mode_var_data_pairs, push those postings to 
//            the info_buffer.

void ModeSet::updateInfoBuffer()
{
  if(!m_info_buffer)
    return;

  unsigned int i, vsize = m_mode_var_data_pairs.size();
  for(i=0; i<vsize; i++) {
    VarDataPair pair = m_mode_var_data_pairs[i];
    string varname = pair.get_var();
    if(pair.is_string()) {
      string varvalue = pair.get_sdata();
      m_info_buffer->setValue(varname, varvalue);
    }
    else {
      double varvalue = pair.get_ddata();
      m_info_buffer->setValue(varname, varvalue);
    }
  }

}

//------------------------------------------------------------------
// Procedure: evaluate()
//   Purpose: Go through each entry and evaluate the conditions. If the 
//            conditions for an entry is met, the collect the varname and
//            varvalue into a VarDataPair to be added to a vector - the
//            member variable m_mode_var_data_paris, which can be 
//            retrieved on a separate call to this instance.

void ModeSet::evaluate()
{
  // Each new call to evaluate wipes out whatever was determined from 
  // previous calls.
  m_mode_var_data_pairs.clear();

  // Accommodate new information in the info_buffer and propogate it 
  // down to each entry's vector of conditions.
  consultFromInfoBuffer();

  // Collect results initially into a map. Mappings may be over-written
  // on later entries. We use a map as a convenient structure for this.
  map<string, string> var_val_map;

  unsigned int i, j, vsize = m_entries.size();
  for(i=0; i<vsize; i++) {
    bool   result  = m_entries[i].evalConditions();
    string varname = m_entries[i].getModeVarName();
    string prefix  = m_entries[i].getModePrefix();
    string elseval = m_entries[i].getModeVarElseValue();
    string varvalue = "";
    if(result) 
      varvalue = m_entries[i].getModeVarValue();
    else {
      if(m_entries[i].evalModeVarConditions())
	varvalue = m_entries[i].getModeVarElseValue();
    }

    if(varvalue != "") {
      if(prefix != "")
	varvalue = (prefix + ":" + varvalue);
      var_val_map[varname] = varvalue;
      // Important to propagate the var-val down to ALL the entries 
      for(j=0; j<vsize; j++)
	m_entries[j].setVarVal(varname, varvalue);
    }

  }

  // Now that the map has been built, create a VarDataPair for each entry.
  cout << endl << "ModeSet::evaluate: " << var_val_map.size() << endl;
  map<string, string>::iterator p;
  for(p=var_val_map.begin(); p!=var_val_map.end(); p++) {
    string varname = p->first;
    string varvalue = p->second;
    
    cout << "  varname:" << varname << "  varval:" << varvalue << endl;
    VarDataPair pair(varname, varvalue, "auto");
    
    // By setting the key we declare that all these pairs are being 
    // generated by the same source. It allows a user to possible *not*
    // process (post) successive pairs with the exact same var-vals.
    pair.set_key("mode_set");
    m_mode_var_data_pairs.push_back(pair);
  }
  cout << endl << "ModeSet::evaluate: done" << endl;

  // Now that we have a bunch of var-value pairs, push this info out to the
  // info buffer.
  updateInfoBuffer();
}


//------------------------------------------------------------------
// Procedure: print()

void ModeSet::print()
{
  cout << "Printing ModeSet - Begin" << endl;
  unsigned int i, vsize = m_entries.size();

  cout << "   Number of ModeEntries: " << vsize << endl;
  for(i=0; i<vsize; i++) {
    cout << "   Mode Entry # " << i << ": " << endl;
    m_entries[i].print();
  }

  vector<string> con_vars = getConditionVars();
  vsize = con_vars.size();
  cout << "   MOOS variables (" << vsize << "): " << endl;

  for(i=0; i<vsize; i++) {
    cout << con_vars[i];
    if(i < (vsize-1))
      cout << ", "; 
    else
      cout << endl;
  }
  cout << "Printing ModeSet - End" << endl;
}


//------------------------------------------------------------------
// Procedure: getModeSummary
//   Example: "MODE_ONE,ACTIVE:MODE_TWO,SURFACE"

string ModeSet::getModeSummary()
{
  string result;

  unsigned int i, vsize = m_mode_var_data_pairs.size();
  string varname, varvalue;
  for(i=0; i<vsize; i++) {
    VarDataPair pair = m_mode_var_data_pairs[i];
    varname = pair.get_var();
    if(pair.is_string()) 
      varvalue = pair.get_sdata();
    else
      varvalue = dstringCompact(doubleToString(pair.get_ddata()));
    if(result != "")
      result += ":";
    result += (varname + "," + varvalue);
  }
  return(result);
}


//------------------------------------------------------------------
// Procedure: getConditionVars

vector<string> ModeSet::getConditionVars()
{
  vector<string> union_vector;
  
  unsigned int i, vsize = m_entries.size();
  for(i=0; i<vsize; i++) {
    vector<string> svector = m_entries[i].getConditionVars();
    union_vector = mergeVectors(union_vector, svector);
  }

  union_vector = removeDuplicates(union_vector);
  return(union_vector);
}
