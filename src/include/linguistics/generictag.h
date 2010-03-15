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

class CStringTokenizer : public CTokenizer<string> {
   public: 
      CStringTokenizer() : CTokenizer<string>(3/*reserve for NONE BEGIN END*/) {} 
      virtual ~CStringTokenizer() {}
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
   static unsigned long COUNT;
   static unsigned long COUNT_BITS;
   static unsigned long LAST;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CStringTokenizer &getTokenizer() const {static CStringTokenizer tokenizer; return tokenizer;}

public:
   CWord() { clear(); }
   CWord(const string &s) { m_code=getTokenizer().lookup(s); }
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

   void load(const string &s) { m_code = getTokenizer().lookup(s); }
   const string &str() const { return getTokenizer().key(m_code); }

   void clear() { m_code=NONE; }
}; 

//===============================================================

inline unsigned long hash(const CWord &w) {return w.hash();}

#endif

