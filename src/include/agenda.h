// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * agenda.h - the agenda classes for seach algorithms.          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _AGENDA_H
#define _AGENDA_H

#include "definitions.h"

#include <algorithm>

/*===============================================================
 *
 * CAgendaSimple - an agenda 
 *
 * CNode needs to be provided to the agenda. It must be comparable
 *
 *==============================================================*/

template <typename CNode>
class CAgendaSimple {
   private:
      const int m_nMaxSize;
      CNode** m_lBeamPointer;
      CNode* m_lBeam;
      int m_nBeamSize;
      int m_nIndex;
      bool m_bItemSorted; 
   public:
      // constructor and destructor method
      CAgendaSimple(int nBeamSize): m_nMaxSize(nBeamSize) { 
         TRACE("Initialising CAgendaSimple");
         m_lBeam = new CNode[nBeamSize]; 
         m_lBeamPointer = new CNode*[nBeamSize]; 
         CNode* pNode;
         pNode = m_lBeam;
         int i;
         for (i=0; i<nBeamSize; ++i) {
            m_lBeamPointer[i] = pNode++;
         }
         clear();  
         TRACE("CAgendaSimple initialised.");
      };
      ~CAgendaSimple() { delete[] m_lBeam; delete[] m_lBeamPointer; }
      // public methods for the agenda manipulation
      void clear() { 
         m_nBeamSize = 0; 
         m_bItemSorted = false;
      }
      const CNode* item(int index) { assert(index<m_nBeamSize); return m_lBeamPointer[index]; }
      int size() { return m_nBeamSize; }
//      CNode* newItem() {
//         assert( !m_bItemSorted ); 
//         if (m_nBeamSize == m_nMaxSize) { 
//            std::pop_heap(m_lBeamPointer, m_lBeamPointer+m_nMaxSize, more); 
//            return m_lBeamPointer[m_nMaxSize-1]; 
//         }
//         return m_lBeamPointer[m_nBeamSize++];
//      }
//      void insertNewItem() { 
//         assert( !m_bItemSorted );
//         std::push_heap(m_lBeamPointer, m_lBeamPointer+m_nBeamSize, more ); 
//      }
      void insertItem(const CNode *item) {
         assert( !m_bItemSorted );
         if (m_nBeamSize == m_nMaxSize) {
            if (*item > *(m_lBeamPointer[0])) {
               std::pop_heap(m_lBeamPointer, m_lBeamPointer+m_nMaxSize, more);
               *(m_lBeamPointer[m_nMaxSize-1]) = *item;
               std::push_heap(m_lBeamPointer, m_lBeamPointer+m_nMaxSize, more);
            }
            return;
         }
         *(m_lBeamPointer[m_nBeamSize]) = *item;
         ++ m_nBeamSize;
         std::push_heap(m_lBeamPointer, m_lBeamPointer+m_nBeamSize, more);
      }
      void sortItems() { 
         assert(!m_bItemSorted); 
         std::sort_heap( m_lBeamPointer, m_lBeamPointer + m_nBeamSize , more ) ; 
         m_bItemSorted = true;
      }
      const CNode* bestItem( int index=0 ) { if (!m_bItemSorted) sortItems(); return m_lBeamPointer[ index ] ; }

      inline static bool more(CNode *x, CNode *y) {return *x > *y;}
      inline static bool less(CNode *x, CNode *y) {return *x < *y;}
};

/*===============================================================
 *
 * CAgendaBeam - an agenda for the beam algorithm
 *
 * CNode needs to be provided to the agenda. It must be comparable
 *
 * The agenda keeps the beam size n-best list.
 * The list is not ordered, but form one heap. In this way the smallest
 * itmes can be discarded when necessary. 
 *
 * Two beams are used, from which one beam is used as the generator
 * beam and the other is used as the generated beam. Public method
 * generatorStart() returns the first item in the generator beam and 
 * generatorSize() returns the size of the generator beam. 
 * candidateItem() returns a candidate item 
 * new status, and pushCandidate() pushes the new status into the
 * generated beam. nextRound() switches the generated beam is turned
 * into the generation beam and the generation beam is cleared and
 * turned into the generated beam. 
 *
 * Typical Usage: CNode* pNode=generatorStart(); 
 *                for (int i=0; i<generatorSize(); i+=1) {
 *                   // for each new possibility
 *                   CNode* pNew=candidateItem();
 *                   // initialise candidate
 *                   pushCandidate();
 *                   pNode=generatorNext();
 *                }
 *                nextRound();
 *
 *==============================================================*/

template <typename CNode>
class CAgendaBeam {
   private:
      const int m_nMaxSize;
      CNode** m_lBeamPointer[2];
      CNode* m_lBeam[2];
      int m_nBeamSize[2];
      unsigned m_nGenerator, m_nGenerated;
      int m_nGeneratorIndex;
   public:
      // constructor and destructor method
      CAgendaBeam(int nBeamSize): m_nMaxSize(nBeamSize) { 
         TRACE("Initialising CAgendaBeam");
         m_lBeam[0] = new CNode[nBeamSize]; m_lBeam[1] = new CNode[nBeamSize]; 
         m_lBeamPointer[0] = new CNode*[nBeamSize]; m_lBeamPointer[1] = new CNode*[nBeamSize]; // don't say new (CNode*)[n], for it will generate "error: array bound forbidden after parenthesized type-id". 
         CNode* pNode;
         pNode = m_lBeam[0];
         int i;
         for (i=0; i<nBeamSize; ++i) {
            m_lBeamPointer[0][i] = pNode++;
         }
         pNode = m_lBeam[1];
         for (i=0; i<nBeamSize; ++i) {
            m_lBeamPointer[1][i] = pNode++;
         }
         clear();  
         TRACE("CAgendaBeam initialised.");
      };
      ~CAgendaBeam() { for(int m=0; m<2; m++) { delete[] m_lBeam[m]; delete[] m_lBeamPointer[m]; } }
      // public methods for the agenda manipulation
      void clear(int which=2) { 
         // when not providing any parameters, clear restores both agenda and initialises the pointers.
         if (which == 2) { m_nBeamSize[0] = 0; m_nBeamSize[1] = 0; m_nGenerator = 0; m_nGenerated = 1; } 
         // when the parameter indicates one agenda, only this is cleaned. 
         else m_nBeamSize[which] = 0; 
      }
      CNode* generatorStart() { if (m_nBeamSize[m_nGenerator]!=0) { m_nGeneratorIndex=0; return m_lBeamPointer[m_nGenerator][m_nGeneratorIndex]; } else return 0; }
      CNode* generatorNext() { 
         ++m_nGeneratorIndex;
         if (m_nGeneratorIndex>=m_nBeamSize[m_nGenerator])
            return NULL; 
         return m_lBeamPointer[m_nGenerator][m_nGeneratorIndex]; 
      }
      int generatorSize() { return m_nBeamSize[m_nGenerator]; }
      int candidateSize() { return m_nBeamSize[m_nGenerated]; }
      CNode* candidateItem() {
         if (m_nBeamSize[m_nGenerated] == m_nMaxSize) { // if reach beam limits
            std::pop_heap(m_lBeamPointer[m_nGenerated], m_lBeamPointer[m_nGenerated]+m_nMaxSize, more); // pop the smallest item
            return m_lBeamPointer[m_nGenerated][m_nMaxSize-1]; // and reuse it.
         }
         // increase the beam size, then return pointer
         return m_lBeamPointer[m_nGenerated][m_nBeamSize[m_nGenerated]++];
      }
      void pushCandidate() { 
         std::push_heap(m_lBeamPointer[m_nGenerated], m_lBeamPointer[m_nGenerated]+m_nBeamSize[m_nGenerated], more ); 
      }
      void pushCandidate(const CNode *node) { 
         if (m_nBeamSize[m_nGenerated] == m_nMaxSize) { // if reach beam limits
            if ( ! ( *node > *(m_lBeamPointer[m_nGenerated][0]) ) )
               return;
            std::pop_heap(m_lBeamPointer[m_nGenerated], m_lBeamPointer[m_nGenerated]+m_nMaxSize, more); // pop the smallest item
         }
         else {
            m_nBeamSize[m_nGenerated]++;
         }
         *(m_lBeamPointer[m_nGenerated][m_nBeamSize[m_nGenerated]-1]) = *node;
         //e[m_nGenerated]++;
         std::push_heap(m_lBeamPointer[m_nGenerated], m_lBeamPointer[m_nGenerated]+m_nBeamSize[m_nGenerated], more ); 
      }
      void nextRound() { m_nGenerator = 1-m_nGenerator; m_nGenerated = 1-m_nGenerated; clear(m_nGenerated); }
      CNode* bestGenerator() { assert(m_nBeamSize[m_nGenerator]!=0); return * std::max_element(m_lBeamPointer[m_nGenerator], m_lBeamPointer[m_nGenerator]+m_nBeamSize[m_nGenerator], less); }
      CNode* generator(int n) { if (n>=m_nBeamSize[m_nGenerator]) return 0; return m_lBeamPointer[m_nGenerator][n]; }
      void sortGenerators() { std::sort_heap(m_lBeamPointer[m_nGenerator], m_lBeamPointer[m_nGenerator]+m_nBeamSize[m_nGenerator], more); }

      void clearGenerators() { clear(m_nGenerator); }
      void clearCandidates() { clear(m_nGenerated); }

      inline static bool more(CNode *x, CNode *y) {return *x > *y;}
      inline static bool less(CNode *x, CNode *y) {return *x < *y;}

};

/*===============================================================
 *
 * CAgendaBeamDoubleIndice - another implementation
 *
 * This one was used by the agenda implementation of segmentor
 *
 *===============================================================*/

//
// Note that this implementation had some obvious problems, since
// the max pointr does not actually order candidates from the big
// scores to the small scores - it only garantees that the best
// item is found first. 
//
template <typename CNode>
class CAgendaBeamDoubleIndice {
   private:
      const int m_nMaxSize;
      CNode** m_lBeamPointer[2];
      CNode** m_lMaxPointer[2];
      CNode* m_lBeam[2];
      int m_nBeamSize[2];
      unsigned m_nGenerator, m_nGenerated;
      int m_nGeneratorIndex;
   public:
      // constructor and destructor method
      CAgendaBeamDoubleIndice(int nBeamSize): m_nMaxSize(nBeamSize) { 
         TRACE("Initialising CAgendaBeam with the max indice");
         m_lBeam[0] = new CNode[nBeamSize]; m_lBeam[1] = new CNode[nBeamSize]; 
         m_lBeamPointer[0] = new CNode*[nBeamSize]; m_lBeamPointer[1] = new CNode*[nBeamSize]; // don't say new (CNode*)[n], for it will generate "error: array bound forbidden after parenthesized type-id". 
         m_lMaxPointer[0] = new CNode*[nBeamSize]; m_lMaxPointer[1] = new CNode*[nBeamSize];
         CNode* pNode;
         pNode = m_lBeam[0];
         int i;
         for (i=0; i<nBeamSize; ++i) {
            m_lBeamPointer[0][i] = pNode++;
            m_lMaxPointer[0][i] = 0;
         }
         pNode = m_lBeam[1];
         for (i=0; i<nBeamSize; ++i) {
            m_lBeamPointer[1][i] = pNode++;
            m_lMaxPointer[1][i] = 0;
         }
         clear();  
         TRACE("CAgendaBeam with the max indice initialised.");
      };
      ~CAgendaBeamDoubleIndice() { for(int m=0; m<2; m++) { delete[] m_lBeam[m]; delete[] m_lBeamPointer[m]; delete[] m_lMaxPointer[m]; } }
      // public methods for the agenda manipulation
      void clear(int which=2) { 
         // when not providing any parameters, clear restores both agenda and initialises the pointers.
         if (which == 2) { m_nBeamSize[0] = 0; m_nBeamSize[1] = 0; m_nGenerator = 0; m_nGenerated = 1; } 
         // when the parameter indicates one agenda, only this is cleaned. 
         else m_nBeamSize[which] = 0; 
      }
      CNode* generatorStart() { m_nGeneratorIndex=0; return m_lMaxPointer[m_nGenerator][m_nGeneratorIndex]; }//return m_lBeamPointer[m_nGenerator][m_nGeneratorIndex]; }
      CNode* generatorNext() { return m_lMaxPointer[m_nGenerator][++m_nGeneratorIndex]; }//m_lBeamPointer[m_nGenerator][++m_nGeneratorIndex]; }
      int generatorSize() { return m_nBeamSize[m_nGenerator]; }
      CNode* candidateItem() {
         if (m_nBeamSize[m_nGenerated] == m_nMaxSize) { // if reach beam limits
            std::pop_heap(m_lBeamPointer[m_nGenerated], m_lBeamPointer[m_nGenerated]+m_nMaxSize, more); // pop the smallest item
            return m_lBeamPointer[m_nGenerated][m_nMaxSize-1]; // and reuse it.
         }
         // increase the beam size, then return pointer
         m_lMaxPointer[m_nGenerated][m_nBeamSize[m_nGenerated]] = m_lBeamPointer[m_nGenerated][m_nBeamSize[m_nGenerated]];
         return m_lBeamPointer[m_nGenerated][m_nBeamSize[m_nGenerated]++];
      }
      void pushCandidate() { 
         std::push_heap(m_lBeamPointer[m_nGenerated], m_lBeamPointer[m_nGenerated]+m_nBeamSize[m_nGenerated], more ); 
         if (m_nBeamSize[m_nGenerated] == m_nMaxSize)
            make_heap(m_lMaxPointer[m_nGenerated], m_lMaxPointer[m_nGenerated]+m_nMaxSize, less);
         else
            std::push_heap(m_lMaxPointer[m_nGenerated], m_lMaxPointer[m_nGenerated]+m_nBeamSize[m_nGenerated], less);
      }
      void nextRound() { m_nGenerator = 1-m_nGenerator; m_nGenerated = 1-m_nGenerated; clear(m_nGenerated); }
      CNode* bestGenerator() { return m_lMaxPointer[m_nGenerator][0]; }//return * std::max_element(m_lBeamPointer[m_nGenerator], m_lBeamPointer[m_nGenerator]+m_nBeamSize[m_nGenerator], less); }
      CNode* generator(int n) { return m_lMaxPointer[m_nGenerator][n]; }//return * std::max_element(m_lBeamPointer[m_nGenerator], m_lBeamPointer[m_nGenerator]+m_nBeamSize[m_nGenerator], less); }

      inline static bool more(CNode *x, CNode *y) {return *x > *y;}
      inline static bool less(CNode *x, CNode *y) {return *x < *y;}

};

/*
 // This is the implementation of traditional agenda - all items are put in the same beam. 
template <typename CNode>
class CAgendaBeam1 {
   private:
      const int m_nMaxSize;
      std::vector<CNode> m_lBeam[2];
      unsigned m_nGenerator, m_nGenerated;
      typename std::vector<CNode>::iterator m_it, m_itemp;
   public:
      // constructor and destructor method
      CAgendaBeam1(int nBeamSize): m_nMaxSize(nBeamSize) { 
         TRACE("Initialising CAgendaBeam");
         clear();  
         TRACE("CAgendaBeam initialised.");
      };
      ~CAgendaBeam1() { }
      // public methods for the agenda manipulation
      void clear(int which=2) { 
         // when not providing any parameters, clear restores both agenda and initialises the pointers.
         if (which == 2) { m_lBeam[0].clear(); m_lBeam[1].clear(); m_nGenerator = 0; m_nGenerated = 1; } 
         // when the parameter indicates one agenda, only this is cleaned. 
         else m_lBeam[which].clear(); 
      }
      CNode* generatorStart() { m_it = m_lBeam[m_nGenerator].begin(); return &(*m_it); }
      CNode* generatorNext() { return &*(++m_it); }
      int generatorSize() { return m_lBeam[m_nGenerator].size(); }
      CNode* candidateItem() {
         CNode node;
         m_lBeam[m_nGenerated].push_back(node);
         return &*m_lBeam[m_nGenerated].rbegin();
      }
      void pushCandidate() { }
      void nextRound() { 
         m_nGenerator = 1-m_nGenerator; 
         m_nGenerated = 1-m_nGenerated; 
         sort(m_lBeam[m_nGenerator].begin(), m_lBeam[m_nGenerator].end());
         if (m_lBeam[m_nGenerator].size() > m_nMaxSize) {
            m_itemp = m_lBeam[m_nGenerator].begin() + m_nMaxSize;
            m_lBeam[m_nGenerator].erase(m_itemp, m_lBeam[m_nGenerator].end());
         }
         clear(m_nGenerated); 
      }
      CNode* bestGenerator() { return &*std::max_element(m_lBeam[m_nGenerator].begin(), m_lBeam[m_nGenerator].end()); }

      static bool more(CNode x, CNode y) {return x > y;}

};
*/

#endif
