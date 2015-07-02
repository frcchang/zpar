// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * stateitem.h - the state items for the Chinese segmentor.     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

namespace chinese {


namespace segmentor {

/*===============================================================
 *
 * CStateItem - the search state item, representing a segmented sentence 
 *              or a part of a complete segmented sentence. 
 *
 *==============================================================*/

class CStateItem {

protected:
   // words 
   long int m_nIndex;
   const CStateItem *m_prev;
   bool m_bAppend;

public:
   // item score
   SCORE_TYPE score;

public:
   // constructors and destructors
   CStateItem() : m_nIndex(-1), m_prev(0), m_bAppend(false) { }
   CStateItem(CStateItem& item) { std::cerr<<"CStateItem does not support copy constructor!"; std::cerr.flush(); assert(1==0); }

public:
   inline void clear() {
      m_nIndex = -1;
      m_prev = 0;
      m_bAppend = false;
   }
   inline bool operator < (const CStateItem &item) const { return this->score < item.score; }
   inline bool operator > (const CStateItem &item) const { return this->score > item.score; }
   inline bool operator == (const CStateItem &item) const { 
      return m_nIndex == item.m_nIndex && m_prev == item.m_prev && m_bAppend == item.m_bAppend;
   }
   inline bool operator != (const CStateItem &item) const { return ! ((*this) == item); }
   inline const long int getWordStart() const { 
      return m_prev==0 ? -1 : m_prev->m_nIndex+1; 
   }
   inline const long int getWordEnd() const { 
      return m_prev==0 ? -1 : m_nIndex; 
   }
   inline const long int getWordLength() const { 
      return m_prev==0 ? 0 : m_nIndex - m_prev->m_nIndex;
   }
   void append(CStateItem *retval) const {
      retval->m_nIndex = m_nIndex+1;
      retval->m_prev = m_prev;
      retval->m_bAppend = true;
   }
   void separate(CStateItem *retval) const {
      retval->m_nIndex = m_nIndex + 1;
      retval->m_prev = this;
      retval->m_bAppend = false;
   }
   inline const bool &isAppended() const {
      return m_bAppend;
   }
   inline const long &index() const {
      return m_nIndex;
   }
   inline const CStateItem *prev() const {
      return m_prev;
   }

   inline bool empty() const {
       return m_nIndex==-1;
   }

};

/*===============================================================
 *
 * CScoredAct
 *
 *==============================================================*/

struct CScoredAct {
public:
   const CStateItem *item;
   bool append;
   SCORE_TYPE score;
public:
   CScoredAct() : item(0), append(false), score(0) {}
   CScoredAct(const CStateItem *st, bool append, SCORE_TYPE score) : item(st), append(append), score(st->score+score) {}
   void load(const CStateItem *st, bool append, SCORE_TYPE score) {
      item = st;
      this->append = append;
      this->score = st->score + score;
   }

   bool operator > (const CScoredAct &a) const { return score > a.score; }
   bool operator < (const CScoredAct &a) const { return score < a.score; }
};

}
}
