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

struct CStateItem {
   // words 
   unsigned long int *m_lWords;                      // record the end of each word
   unsigned long int m_nLength;                      // the length of the sentence means the number of words it contains
//   unsigned char m_nLastWord;                   // the status of the last word: 0 - no use; 1 - end of a word; 2 - not end

   // item score
   SCORE_TYPE m_nScore;
   CStateItem() { m_lWords = new unsigned long int[MAX_SENTENCE_SIZE]; clear(); }
   ~CStateItem() { delete[] m_lWords; }
   CStateItem(CStateItem& item) { std::cerr<<"CStateItem does not support copy constructor!"; std::cerr.flush(); assert(1==0); }
   //inline bool operator < (const CStateItem &item) const { return this->m_nScore < item.m_nScore ? true : this->m_nScore == item.m_nScore && this->m_nLength < item.m_nLength; }
   //inline bool operator > (const CStateItem &item) const { return this->m_nScore > item.m_nScore ? true : this->m_nScore == item.m_nScore && this->m_nLength > item.m_nLength; }
   // These definitions are better for the agenda segmentor. 
   inline bool operator < (const CStateItem &item) const { return this->m_nScore < item.m_nScore; }
   inline bool operator > (const CStateItem &item) const { return this->m_nScore > item.m_nScore; }
   inline bool operator == (const CStateItem &item) const { 
      if ( this->m_nLength != item.m_nLength )
         return false;
      for ( int i = 0; i < this->m_nLength; ++i ) 
         if ( m_lWords[i] != item.m_lWords[i] )
            return false;
      return true; 
   }
   inline bool operator != (const CStateItem &item) const { return ! ((*this) == item); }
   inline void operator = (const CStateItem &item) { copy(&item); }
   inline const unsigned long int getWordStart(const unsigned long int i) const { assert(i<m_nLength); return i>0?m_lWords[i-1]+1:0; }
   inline const unsigned long int getWordEnd(const unsigned long int i) const { assert(i<m_nLength); return m_lWords[i]; }
   inline const unsigned long int getWordLength(const unsigned long int i) const { assert(i<m_nLength); return i>0?m_lWords[i]-m_lWords[i-1]:m_lWords[0]+1;}
   void copy(const CStateItem *from) {
      m_nLength = from->m_nLength;
      m_nScore = from->m_nScore;
      for (int i=0; i<m_nLength; ++i) {
         m_lWords[i] = from->m_lWords[i];
      }
//      m_nLastWord = from->m_nLastWord;
   }
   void append(unsigned long int char_index) {
      m_lWords[m_nLength] = char_index;
      ++m_nLength ;
//      m_nLastWord = 0 ;
   }
   void replace(unsigned long int char_index) {
      m_lWords[m_nLength-1] = char_index; 
//      m_nLastWord = 0 ;
   }
//   void setWordEnd(bool bEnd = true) { // set the status of the last word ...
//      m_nLastWord = bEnd ? 1 : 2; 
//   }
//   bool canSeparate() const { // the next char can be separated from the last word?
//      return m_nLastWord != 2 ;
//   }
//   bool canAppend() const { // the next char can be appended to the last word?
//      return m_nLastWord != 1 ; 
//   }
//   bool isCompatibleWith(const CStateItem *item) const {
//      if (item->m_nLength==0) return true;
//      unsigned long int length = std::min(m_nLength, item->m_nLength);
//      for (int i=0; i<length-1; ++i)
//         if (m_lWords[i] != item->m_lWords[i]) return false;
//      if (m_lWords[length-1]<item->m_lWords[length-1])
//         return false;
//      else if (m_lWords[length-1]==item->m_lWords[length-1])
//         return this->canSeparate() == item->canSeparate() ;
//      return true;
//   }
   inline void clear() { m_nScore = 0; m_nLength = 0; } //m_nLastWord = 0 ; }
};

}
}
