// Copyright (C) University of Oxford 2010
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
   unsigned long *m_lWords;
   CTag *m_lTags;
   unsigned long m_nLength; // the length of the sentence means the number of words it contains

   // action
   enum ACTION_TYPE { eNoAction=0, eAppend, eSeparate };
   ACTION_TYPE m_nAction;

public:

   // item score
   SCORE_TYPE score;

public:

   // constructor and destructor

   CStateItem() {
      m_lWords = new unsigned long [MAX_SENTENCE_SIZE];
      m_lTags = new CTag[MAX_SENTENCE_SIZE];
      clear();
   }
   ~CStateItem() {
      delete[] m_lWords;
      delete[] m_lTags;
   }

   CStateItem(CStateItem& item) {
      THROW("CStateItem does not support copy constructor!");
   }

public:

   // comparison and assignment

   inline bool operator < (const CStateItem &item) const { return this->score < item.score ? true : false; /*this->score == item.score && this->m_nLength < item.m_nLength; */}

   inline bool operator > (const CStateItem &item) const { return this->score > item.score ? true : false; /*this->score == item.score && this->m_nLength > item.m_nLength; */}

   bool operator == (const CStateItem &item) const {
      if ( m_nLength != item.m_nLength ) return false;
      for ( int i=0; i<m_nLength; ++i ) {
         if ( m_lWords[i] != item.m_lWords[i] ) return false;
         if ( m_lTags[i] != item.m_lTags[i] ) return false;
      }
      return true;
   }

   bool operator != (const CStateItem &item) const {
      return !((*this)==item);
   }

   void copy(const CStateItem *from) {
      m_nLength = from->m_nLength;
      score = from->score;
      for (int i=0; i<m_nLength; ++i) {
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

   inline const unsigned long getWordStart(const unsigned long &i) const { return i>0?m_lWords[i-1]+1:0; }

   inline const unsigned long getWordEnd(const unsigned long &i) const { return m_lWords[i]; }

   inline const unsigned long getWordLength(const unsigned long &i) const { return i>0?m_lWords[i]-m_lWords[i-1]:m_lWords[0]+1;}

   inline const unsigned long size() const { return m_nLength; }

   // the traditional interface

   void append(const unsigned long &char_index, const unsigned long &tag) {
      m_lWords[m_nLength] = char_index;
      m_lTags[m_nLength].load(tag);
      ++m_nLength ;
   }

   inline void replace(const unsigned long &char_index, const unsigned long &tag) {
      m_lWords[m_nLength-1] = char_index;
      m_lTags[m_nLength-1].load(tag);
   }

   inline void replaceIndex(const unsigned long &char_index) {
      m_lWords[m_nLength-1] = char_index;
   }

   inline void setTag(const unsigned long &index, const unsigned long &tag) {
      m_lTags[index].load(tag);
   }

   inline const CTag &getTag(const unsigned long &index) const {
      return m_lTags[index];
   }

public:

   // action based - appending / separating ?

   const CTag &getLastTag() const {
      const static CTag begin(CTag::SENTENCE_BEGIN);
      const static CTag none(CTag::NONE);
      if ( m_nLength == 0 ) return begin ;
      if ( m_lTags[m_nLength-1] == none ) {
         assert( m_nAction == eAppend ) ;
         if ( m_nLength == 1 ) return begin ;
         assert( m_lTags[m_nLength-2] != none ) ;
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
      for (int i=0; i<length-1; ++i) {
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
