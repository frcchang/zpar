// Copyright (C) University of Oxford 2010

#ifndef _CHINESE_TAGGER_SEGMENTATIONPRUNE_H
#define _CHINESE_TAGGER_SEGMENTATIONPRUNE_H

/*===============================================================
 *
 * CSegmentationPrune - the pruning structure for the segmenting
 *
 *==============================================================*/

class CSegmentationPrune {

protected:
   int m_nMaxSentenceSize;
   unsigned char *m_chars;

public:
   enum PRUNE { eNoAction = 0, eAppend, eSeparate }; 

public:
   CSegmentationPrune(int nMaxSentenceSize) { 
      m_nMaxSentenceSize = nMaxSentenceSize; 
      m_chars = new unsigned char[nMaxSentenceSize]; 
      reset(); 
   }
   virtual ~CSegmentationPrune() {
      delete [] m_chars;
   }

public:
   // this method will produce a new sentence that does not contain spa
   void load( const CStringVector &sentence_input, CStringVector &sentence) {
      sentence.clear();
      reset();

      int index_out = 0; 
      for ( int index_in = 0; index_in < sentence_input.size(); index_in++ ) {
         const std::string &current_char = sentence_input[index_in];
         const std::string &last_char = index_in>0 ? sentence_input[index_in-1] : "" ;
         if ( current_char != " " ) {
            sentence.push_back( current_char );
            if ( index_in>0 && last_char == " " )
               segment( index_out, true ) ;
            index_out++ ;
         }
      }
   }

public:
   void segment(unsigned long int index, bool separate) { m_chars[index] = ( separate ? eSeparate : eAppend ) ; }
   bool canSeparate(unsigned long int index) { return m_chars[index] == eNoAction || m_chars[index] == eSeparate; }
   bool canAppend(unsigned long int index) { return m_chars[index] == eNoAction || m_chars[index] == eAppend; }
   void reset() { for (int i=0; i<m_nMaxSentenceSize; ++i) m_chars[i] = 0; }
};

#endif
