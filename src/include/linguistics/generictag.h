/****************************************************************
 *                                                              *
 * tokenizedword.h - the definitions for tokenized words        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _WORD_H
#define _WORD_H

#include "hash.h"
#include "tokenizer.h"
#include "bigram.h"

/*===============================================================
 *
 * the tokenizer
 *
 *==============================================================*/

class CTagTokenizer : public CTokenizer<string> {
   public: 
      CTagTokenizer() : CTokenizer<string>(3/*reserve for NONE BEGIN END*/) {} 
      virtual ~CTagTokenizer() {}
};

/*===============================================================
 *
 * definitions about tag 
 *
 *==============================================================*/

class CTag {
public:
   enum {NONE = 0};
   enum {SENTENCE_BEGIN = 1};
   enum {SENTENCE_END = 2};
   enum {FIRST = 3};
   static unsigned long COUNT = 3;
   static unsigned long COUNT_BITS = 2;
   static unsigned long LAST = 2;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CTagTokenizer &getTokenizer() const {static CTagTokenizer tokenizer; return tokenizer;}

public:
   CWord() { clear(); }
   CWord(const string &s) { load(s); }
   CWord(const CTag &t) { m_code=t.m_code; }
   CWord(const unsigned &u) { assert(getTokenizer().count()>u); m_code=u; }
   virtual ~CWord() {}

public:
   unsigned long code() const { return m_code; }
   bool operator == (const CWord &w) const { return m_code == w.m_code; }
   bool operator != (const CWord &w) const { return m_code != w.m_code; }
   bool operator < (const CWord &w) const { return m_code < w.m_code; }
   bool operator > (const CWord &w) const { return m_code > w.m_code; }
   bool operator <= (const CWord &w) const { return m_code <= w.m_code; }
   bool operator >= (const CWord &w) const { return m_code >= w.m_code; }

   void load(const string &s) {
      m_code=getTokenizer().lookup(s); 
      if (m_code>LAST) { 
         assert(m_code == LAST+1);
         ++COUNT; 
         if ((1<<COUNT_BITS)>COUNT) ++COUNT_BITS;
         ++LAST;
         TRACE("Tag updated: count="<<COUNT<<",countbits="<<COUNT_BITS<<",last="<<LAST<<".");
      } 
   }
   const string &str() const { return getTokenizer().key(m_code); }

   void clear() { m_code=NONE; }
}; 

//===============================================================

inline unsigned long hash(const CWord &w) {return w.hash();}

#endif

