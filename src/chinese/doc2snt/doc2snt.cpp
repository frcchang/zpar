// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * doc2snt.cpp - the chinese document to sentence converter     *
 *                                                              *
 * This implementation uses beam search ageanda, see chart.cpp  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2009.8                         *
 *                                                              *
 ****************************************************************/

#include "doc2snt.h"

using namespace chinese;

/*---------------------------------------------------------------
 *
 * fillBuffer - fill buffer
 *
 *--------------------------------------------------------------*/

bool CDoc2Snt::fillBuffer() {
   std::string chr;
   bool retval = false;
   // keep on reading
   while (!m_buffer.full() && m_reader->readRawCharacter(&chr)) {
      retval = true;
      m_buffer.pushBack(chr);
      // reading is segmented by EOL
      if (chr=="\n") break;
   }
   return retval;
}

/*---------------------------------------------------------------
 *
 * getSentence - read sentence
 *
 * Find a sentence that ends with a punctuation, \n, or 
 * maximum size. \n is always returned as a single sent
 *
 *--------------------------------------------------------------*/

bool CDoc2Snt::getSentence(CStringVector &retval) {
   // the reading is segmented by \n; otherwise it will wait until stdin fills
   if ( m_buffer.size() == 0 || m_buffer[m_buffer.size()-1] != "\n" ) {
      if ( !fillBuffer() ) return false;
   }
   unsigned long index = 0;
   while (index<m_buffer.size()) {
      if ( index==m_buffer.size()-1 || 
           isSentenceSeparator(m_buffer[index]) || 
           m_buffer[index]=="\n" ) { //
         // continued punctuations, eg ."    )"    '" etc
         while (index < m_buffer.size()-1 &&
                isPunctuation(m_buffer[index]) && 
                (isPunctuation(m_buffer[index+1])||m_buffer[index+1]=="\n")    //&&
                ) 
            ++ index;
         // copy buffer
         for (unsigned long i=0; i<=index; ++i) {
            retval.push_back(m_buffer[i]);
         }
         ++index;
         break;
      } 
      ++index;
   }
   m_buffer.popFront(index);
   return true;
}
