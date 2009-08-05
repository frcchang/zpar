#ifndef _CHINESE_TAGGER_STATEITEM_H
#define _CHINESE_TAGGER_STATEITEM_H

namespace chinese {
namespace tagger{
/*===============================================================
 *
 * CStateItem - the search state item, representing a tagged sentence 
 *              or a part of a complete tagged sentence. 
 *
 * m_lWords stores the ending positions of each word. 
 *
 * For example: suppose that a sentence 
 *              A B C D E F
 *              is segmented into
 *              A B C | D E | F
 *              it's corresponding m_lWords is
 *              {2,4,5}
 *
 *==============================================================*/

class CStateItem {

protected:
   // words and tags
   unsigned *m_lWords; 
   unsigned *m_lTags;
   unsigned m_nLength; // the length of the sentence means the number of words it contains

   // action
   enum ACTION_TYPE { eNoAction=0, eAppend, eSeparate };
   ACTION_TYPE m_nAction;

public:

   // item score
   SCORE_TYPE score;

public:

   // constructor and destructor

   CStateItem() { m_lWords = new unsigned [MAX_SENTENCE_SIZE]; m_lTags = new unsigned[MAX_SENTENCE_SIZE]; clear(); }
   ~CStateItem() { delete[] m_lWords; delete[] m_lTags; }
   CStateItem(CStateItem& item) { cerr<<"CStateItem does not support copy constructor!"; cerr.flush(); assert(1==0); }

public:

   // comparison and assignment

   inline bool operator < (const CStateItem &item) const { return this->score < item.score ? true : false; /*this->score == item.score && this->m_nLength < item.m_nLength; */}

   inline bool operator > (const CStateItem &item) const { return this->score > item.score ? true : false; /*this->score == item.score && this->m_nLength > item.m_nLength; */}

   bool operator == (const CStateItem &item) {
      if ( m_nLength != item.m_nLength ) return false;
      for ( int i=0; i<m_nLength; i++ ) {
         if ( m_lWords[i] != item.m_lWords[i] ) return false;
         if ( m_lTags[i] != item.m_lTags[i] ) return false;
      }
      return true;
   }

   bool operator != (const CStateItem &item) {
      return !((*this)==item);
   }

   void copy(const CStateItem *from) {
      m_nLength = from->m_nLength;
      score = from->score;
      for (int i=0; i<m_nLength; i++) {
         m_lWords[i] = from->m_lWords[i];
         m_lTags[i] = from->m_lTags[i];
      }
      m_nAction = from->m_nAction;
   }

   void operator = (const CStateItem &from) {
      this->copy(&from);
   }

   inline void clear() { score = 0; m_nLength = 0; m_nAction = eNoAction; }

public:

   // information about segmentation

   inline const unsigned getWordStart(const unsigned &i) const { return i>0?m_lWords[i-1]+1:0; }

   inline const unsigned getWordEnd(const unsigned &i) const { return m_lWords[i]; }

   inline const unsigned getWordLength(const unsigned &i) const { return i>0?m_lWords[i]-m_lWords[i-1]:m_lWords[0]+1;}

   inline const unsigned size() const { return m_nLength; }

   // the traditional interface

   void append(const unsigned &char_index, const unsigned &tag) {
      m_lWords[m_nLength] = char_index;
      m_lTags[m_nLength] = tag;
      ++m_nLength ;
   }

   void append(const unsigned &char_index) {
      m_lWords[m_nLength] = char_index;
      m_lTags[m_nLength] = PENN_TAG_NONE;
      ++m_nLength ;
   }

   inline void replace(const unsigned &char_index, const unsigned &tag) {
      m_lWords[m_nLength-1] = char_index;
      m_lTags[m_nLength-1] = tag;
   }

   inline void replace(const unsigned &char_index) {
      m_lWords[m_nLength-1] = char_index;
      m_lTags[m_nLength-1] = PENN_TAG_NONE;
   }

   inline void setTag(const unsigned &index, const unsigned &tag) {
      m_lTags[index] = tag;
   }

   inline unsigned getTag(const unsigned &index) const {
      return m_lTags[index];
   }

public:

   // action based - appending / separating ? 

   unsigned getLastTag() {
      if ( m_nLength == 0 ) return PENN_TAG_BEGIN ;
      if ( m_lTags[m_nLength-1] == PENN_TAG_NONE ) {
         assert( m_nAction == eAppend ) ; 
         if ( m_nLength == 1 ) return PENN_TAG_BEGIN ; 
         assert( m_lTags[m_nLength-2] != PENN_TAG_NONE ) ;
         return m_lTags[m_nLength-2] ;
      }
      return m_lTags[m_nLength-1];
   }

   int getLastEnd() {
      if ( m_nLength == 0 ) return -1 ;
      assert( m_nAction != eNoAction ) ;
      if ( m_nAction == eSeparate ) return m_lWords[m_nLength-1] ;
      if ( m_nLength == 1 ) return -1 ;
      return m_lWords[m_nLength-2];
   }

   inline bool canAppend() { return m_nAction != eSeparate; }

   inline bool canSeparate() { return m_nAction != eAppend; }

   inline void setWordEnd( bool bWordEnd ) {
      m_nAction = bWordEnd ? eSeparate : eAppend;
   }

public:

   // this is comparing a correct item with others 

   bool isCompatibleWith(const CStateItem *item) const {
      if (item->m_nLength==0) return true;
      assert( item->m_nLength <= m_nLength );
      const int &length = item->m_nLength;
      for (int i=0; i<length-1; i++) {
         if (m_lWords[i] != item->m_lWords[i]) return false;
         if (m_lTags[i] != item->m_lTags[i]) return false;
      }
      if (m_lWords[length-1]<item->m_lWords[length-1])
         return false;
      else if (m_lWords[length-1]==item->m_lWords[length-1]) {
         assert( m_nAction == eNoAction );
         if ( item->m_nAction == eAppend ) return false;
         if ( m_lTags[length-1]!=item->m_lTags[length-1] ) return false;
      }
      else {
         if ( item->m_nAction == eSeparate ) return false;
      }
      return true;
   }
};

}
}
#endif
