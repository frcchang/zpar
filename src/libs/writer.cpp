// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * writer.cpp - the sentence writer classes definition          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "writer.h"

/*===============================================================
 *
 * CSentenceWriter - write sentence
 *
 *==============================================================*/

void CSentenceWriter::writeLine() {
   (*m_oStream) << std::endl;
};

void CSentenceWriter::writeSentence(const CStringVector* sentence, const std::string &separator, const bool newline) {
   for (int i=0; i<sentence->size(); ++i)
      (*m_oStream) << sentence->at(i) << separator;
   if (newline) (*m_oStream) << std::endl;
};

void CSentenceWriter::writeSentence(const CTwoStringVector* sentence, const char separator, const bool newline) {
   for (int i=0; i<sentence->size(); ++i) {
      if (i>0)
         m_oStream->put(' ');
      for (int j=0; j<sentence->at(i).first.length(); j++)
         m_oStream->put(sentence->at(i).first[j]);
      m_oStream->put(separator);
      for (int j=0; j<sentence->at(i).second.length(); j++)
         m_oStream->put(sentence->at(i).second[j]);
   }
   if (newline) (*m_oStream) << std::endl;
};
 
