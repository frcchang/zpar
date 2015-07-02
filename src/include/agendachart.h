// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * agendachart.h - the mix agenda chart classes for             *
 *                 seach algorithms.                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _AGENDACHART_H
#define _AGENDACHART_H

#include "definitions.h"
#include "agenda.h"

// exception for agenda chart
class CAgendaChartException {};

/*===============================================================
 *
 * CAgendaChart - the chart for one-dimensional dynamic programming 
 *
 * CNode needs to be provided to the chart. It must be comparable
 *
 *==============================================================*/

template <typename CNode>
class CAgendaChart {
   private:
      std::vector< CAgendaSimple<CNode>* > m_vNodes;
      unsigned long int m_nSize;
      const unsigned long int m_nAgendaSize;
   public:
      // constructor and destructor method
      CAgendaChart(int nAgendaSize) : m_nAgendaSize(nAgendaSize) { clear(); }
      ~CAgendaChart() { for(unsigned long int i=0; i<m_vNodes.size(); ++i) delete m_vNodes[i]; }
      // public methods for manipulation
      void clear() { m_nSize = 0; }
      CAgendaSimple<CNode>* operator [] (unsigned long int index) { 
         if (index==m_nSize) { 
            if (index==m_vNodes.size()) m_vNodes.push_back(new CAgendaSimple<CNode>(m_nAgendaSize)); 
            m_vNodes[m_nSize]->clear(); 
            m_nSize++;
         } 
         if (index<m_nSize) 
            return m_vNodes[index]; 
         else 
            throw(new CAgendaChartException); 
      }
};

#endif
