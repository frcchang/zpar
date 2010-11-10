// Copyright (C) University of Oxford 2010
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
      std::ostream *m_oStream;
   public:
      CWriter(std::string sFileName="") { if (sFileName=="") m_oStream=&std::cout; else {m_oStream = new std::ofstream(sFileName.c_str());} };
      virtual ~CWriter() { if (m_oStream != &std::cout) {((std::ofstream*)m_oStream)->close(); delete m_oStream;} };
};

/*===============================================================
 *
 * CSentenceWriter - write sentence
 *
 *==============================================================*/

class CSentenceWriter : public CWriter {
public:
   CSentenceWriter(std::string sFileName="") : CWriter(sFileName) {};
   void writeLine();
   void writeSentence(const CStringVector * sentence, const std::string &separator=" ", const bool newline=true);
   void writeSentence(const CTwoStringVector * sentence, const char separator='_', const bool newline=true);
};

#endif
