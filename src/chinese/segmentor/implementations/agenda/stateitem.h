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
   CStateItem(CStateItem& item) { cerr<<"CStateItem does not support copy constructor!"; cerr.flush(); assert(1==0); }

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
   inline const unsigned long int getWordStart() const { 
      return m_bAppend ? m_prev->m_nIndex : m_nIndex; 
   }
   inline const unsigned long int getWordEnd() const { 
      return m_nIndex; 
   }
   inline const unsigned long int getWordLength() const { 
      return m_bAppend ? m_nIndex - m_prev->m_nIndex + 1 : 1;
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
   inline const CStateItem *&prev() const {
      return m_prev;
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
   CScoredAct(const CStateItem *st, bool append, SCORE_TYPE score) : item(st), append(append), score(st->score+score) {}
   load(const CStateItem *st, bool append, SCORE_TYPE score) {
      item = st;
      this->append = append;
      this->score = st.score + score;
   }

   bool operator < (const CScoredAct &a) const { return score > a.score; }
   bool operator < (const CScoredAct &a) const { return score < a.score; }
}

}
}
