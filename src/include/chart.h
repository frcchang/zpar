// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * chart.h - the chart classes for seach algorithms.            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHART_H
#define _CHART_H

#include "definitions.h"

/*===============================================================
 *
 * CChart - the chart for one-dimensional dynamic programming
 *
 * CNode needs to be provided to the chart. It must be comparable
 *
 *==============================================================*/

template <typename CNode>
class CChart {
   private:
      std::vector<CNode*> m_vNodes;
      int m_nSize;
   public:
      // constructor and destructor method
      CChart() { clear(); }
      ~CChart() { for(int i=0; i<m_vNodes.size(); ++i) delete m_vNodes[i]; }
      // public methods for manipulation
      void clear() { m_nSize = 0; }
      void updateItem(int index, const CNode &item) { if ( index < m_nSize ) { if ( item > *(m_vNodes[index]) ) m_vNodes[index]->copy(&item); else return; } else { if (index==m_nSize) { if (index>=m_vNodes.size()) m_vNodes.push_back(new CNode()); ++m_nSize; m_vNodes[index]->copy(&item); } else throw("Index out of range in addItem method call. You can only add item to existing chart or append to the end."); } }//
      const CNode * operator [] (int index) { return m_vNodes[index]; }
};

#endif
