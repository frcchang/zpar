// Copyright (C) University of Oxford 2010

#ifndef _COMMON_TAGGER_STATEITEM_H
#define _COMMON_TAGGER_STATEITEM_H

/*===============================================================
 *
 * CStateItem - the search state item, representing a tagged sentence 
 *              or a part of a complete tagged sentence. 
 *
 *==============================================================*/

struct CStateItem {
   // words and tags
   const CStateItem *prev;
   unsigned tag;
   // item score
   SCORE_TYPE m_nScore;
   CStateItem() : prev(0), tag() { }
   CStateItem(CStateItem& item) { THROW("CStateItem does not support copy constructor!"); }
   inline bool operator < (const CStateItem &item) const { return this->m_nScore < item.m_nScore; }
   inline bool operator > (const CStateItem &item) const { return this->m_nScore > item.m_nScore; }
   inline SCORE_TYPE &score() { return m_nScore; }
   inline void operator = (const CStateItem &item) {
      prev = item.prev;
      tag = item.tag;
      m_nScore = item.m_nScore;
   }
};
#endif
