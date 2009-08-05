/****************************************************************
 *                                                              *
 * writer.h - the sentence writer classes                       *
 *                                                              *
 * this file is specifically defined for sentence_string        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef WRITER_H
#define WRITER_H

#include "definitions.h"
#include "linguistics/sentence_string.h"

/*===============================================================
 *
 * CWriter - writer prototype 
 *
 *==============================================================*/

class CWriter {
   protected:
      ostream *m_oStream;
   public:
      CWriter(string sFileName="") { if (sFileName=="") m_oStream=&cout; else {m_oStream = new ofstream(sFileName.c_str());} };
      virtual ~CWriter() { if (m_oStream != &cout) {((ofstream*)m_oStream)->close(); delete m_oStream;} };
};

/*===============================================================
 *
 * CSentenceWriter - write sentence
 *
 *==============================================================*/

class CSentenceWriter : public CWriter {
   public:
      CSentenceWriter(string sFileName="") : CWriter(sFileName) {};
      void writeSentence(const CSentenceRaw * sentence);
      void writeSentence(const CSentenceTagged * sentence, const char separator='_');
};

#endif
