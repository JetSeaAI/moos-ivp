/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: StringTree.h                                         */
/*    DATE: Jan 9th 2009                                         */
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

#ifndef STRING_TREE_HEADER
#define STRING_TREE_HEADER

#include <vector>
#include <string>
#include "StringNode.h"

class StringTree
{
public:
  StringTree() {};
  ~StringTree() {};

  bool addParChild(const std::string& parent, 
		   const std::string& child);
  
  void print();
  std::vector<std::string> getPrintableSet();

protected:
  std::vector<StringNode>  m_nodes;
  std::vector<std::string> m_parents_waiting;
  std::vector<std::string> m_children_waiting;
};
#endif


