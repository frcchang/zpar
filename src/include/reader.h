/****************************************************************
 *                                                              *
 * reader.h - the sentence reader classes                       *
 *                                                              *
 * this file is specifically designed for sentence_string       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef READER_H
#define READER_H

#include "definitions.h"
#include "linguistics/sentence_string.h"

/*===============================================================
 *
 * CSentenceReader - read sentence
 *
 * Specify a file name in the constructor. If no file name is specified, 
 * the reader will read from the standard input.
 *
 * readRawSentence:
 *  - The input file should contain tokenised sentences each in a line, 
 *    with space separated words and punctuations. 
 *    In the Chinese case, each character should be separated by space.
 *
 *==============================================================*/

class CSentenceReader {
   protected:
      istream *m_iStream;
      int m_nLine;
   public:
      // constructor and destructor method
      CSentenceReader(const string &sFileName="") { 
         if (sFileName.empty()) 
            m_iStream = &cin; 
         else 
            m_iStream=new ifstream(sFileName.c_str());
         m_nLine = 0;
      };
      virtual ~CSentenceReader() {
         if (m_iStream != &cin) {
            ((ifstream*)m_iStream)->close(); 
            delete m_iStream;
         }
      };
      // this method makes a new instance of CSentenceRaw
      bool readRawSentence(CSentenceRaw *retval, bool bSkipEmptyLines=false, bool bIgnoreSpace=true);
      bool readSegmentedSentence(CSentenceRaw *retval, bool bSkipEmptyLines=false);
      bool readTaggedSentence(CSentenceTagged *retval, bool bSkipEmptyLines=false, const char separator='_');
};

#endif
