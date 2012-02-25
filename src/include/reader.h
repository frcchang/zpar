// Copyright (C) University of Oxford 2010
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
#include "file_utils.h"
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
      std::istream *m_iStream;
      int m_nLine;
   public:
      // constructor and destructor method
      CSentenceReader(const std::string &sFileName="") { 
         if (sFileName.empty()) 
	   m_iStream = &std::cin; 
         else {
            if (!FileExists(sFileName)) THROW("File " << sFileName << " not found.");
            m_iStream=new std::ifstream(sFileName.c_str());
         }
         m_nLine = 0;
      };
      virtual ~CSentenceReader() {
         if (m_iStream != &std::cin) {
            ((std::ifstream*)m_iStream)->close(); 
            delete m_iStream;
         }
      };
      bool readRawCharacter(std::string *retval);
      bool readRawSentence(CStringVector *retval, bool bSkipEmptyLines=false, bool bIgnoreSpace=false);
      bool readSegmentedSentence(CStringVector *retval, bool bSkipEmptyLines=false);
      bool readTaggedSentence(CTwoStringVector *retval, bool bSkipEmptyLines=false, const char separator='_');
      bool readSegmentedSentenceAndTokenize(CStringVector *vReturn, bool bSkipEmptyLines=false);
};

#endif
