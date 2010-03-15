
#ifndef _COMMON_TAGGER_STATEITEM_H
#define _COMMON_TAGGER_STATEITEM_H 1

/*===============================================================
 *
 * CStateItem - the search state item, representing a tagged sentence 
 *              or a part of a complete tagged sentence. 
 *
 *
 *==============================================================*/

struct CStateItem {
   // words and tags
   CTag *m_lTags;
   unsigned long m_nLength;                      // the length of the sentence means the number of words it contains
   // item score
   SCORE_TYPE m_nScore;
   CStateItem() { m_lTags = new CTag[MAX_SENTENCE_SIZE]; clear(); }
   ~CStateItem() { delete[] m_lTags; }
   CStateItem(CStateItem& item) { cerr<<"CStateItem does not support copy constructor!"; cerr.flush(); assert(1==0); }
   inline bool operator < (const CStateItem &item) const { return this->m_nScore < item.m_nScore; }
   inline bool operator > (const CStateItem &item) const { return this->m_nScore > item.m_nScore; }
   void copy(const CStateItem *from) {
      m_nLength = from->m_nLength;
      m_nScore = from->m_nScore;
      for (int i=0; i<m_nLength; ++i) {
         m_lTags[i] = from->m_lTags[i];
      }
   }
   void append(const unsigned long &tag) {
      m_lTags[m_nLength] = tag;
      ++m_nLength ;
   }
   void setTag(const unsigned long &index, const unsigned long &tag) {
      m_lTags[index] = tag;
   }
   const CTag &getTag(const unsigned long &index) const {
      return m_lTags[index];
   }
   inline void clear() { m_nScore = 0; m_nLength = 0; }
   inline SCORE_TYPE &score() { return m_nScore; }
};
#endif
