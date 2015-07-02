// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * doc2snt.h - the document to sentence convertor.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2009.8                         *
 *                                                              *
 ****************************************************************/

#ifndef CHINESE_DOC2SNT_H
#define CHINESE_DOC2SNT_H 

#include "include.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

namespace chinese {

/*===============================================================
 *
 * CDoc2Snt - the sentence boundary detecter for Chinese 
 *
 *==============================================================*/

class CDoc2Snt {

protected:

   const unsigned long m_nMaxSentSize;
   CBufferCycle<std::string> m_buffer;
   CSentenceReader *m_reader;

public:

   CDoc2Snt(const std::string &sFile, unsigned long nMaxSentSize) : m_nMaxSentSize(nMaxSentSize), m_buffer(nMaxSentSize){
      m_reader = new CSentenceReader(sFile);
   };

   virtual ~CDoc2Snt() { 
      delete m_reader;
   };

protected:

   bool fillBuffer();

public:

   // returns whether a sentence is read.
   bool getSentence(CStringVector &retval);

};

} // namespace chinese

#endif


