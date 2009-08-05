/****************************************************************
 *                                                              *
 * tokenizedword.h - the definitions for tokenized words        *
 *                                                              *
 * A faster alternative to word.h                               *
 *                                                              *
 * Pay special attention to unseen words.                       *
 * They are all allocated as g_tokenForUnknownString in the map,*
 * so this shouldn't be used if necessary to distinguish them.  *
 * Using constructor and the method setString has two effects.  *
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
      CStringTokenizer() : CTokenizer<string>(1/*reserve for the other unknown */) {lookup("");} 
      virtual ~CStringTokenizer() {}
};

const unsigned long int g_tokenForUnknownString = 0;
const unsigned long int g_tokenForEmptyString = 1;

/*===============================================================
 *
 * definitions about word 
 *
 *==============================================================*/

class CWord {

protected:
   long int m_nHash;

protected:
   // static method assigns tokenizer as global dictionary of words
   CStringTokenizer &getTokenizer() const {static CStringTokenizer tokenizer; return tokenizer;}

public:
   CWord() { clear(); }
   CWord(const string &s) { static CStringTokenizer &tokenizer = getTokenizer(); m_nHash = tokenizer.lookup(s); }
   CWord(const CWord &w) { m_nHash=w.m_nHash; }
   CWord(const CWord *w) { m_nHash=w->m_nHash; }
   virtual ~CWord() {}

public:
   virtual unsigned long int hash() const { return m_nHash; }
   virtual bool operator == (const CWord &w) const { return m_nHash == w.m_nHash; }
   virtual bool operator != (const CWord &w) const { return m_nHash != w.m_nHash; }
   virtual bool operator < (const CWord &w) const { return m_nHash < w.m_nHash; }
   virtual void operator = (const string &s) { static CStringTokenizer &tokenizer = getTokenizer(); m_nHash = tokenizer.lookup(s); }
   void setString(const string &s) { static CStringTokenizer &tokenizer = getTokenizer(); m_nHash = tokenizer.find(s, g_tokenForUnknownString); }
   // do not use str() for unknown words!!
   inline const string &str() const { static CStringTokenizer &tokenizer = getTokenizer(); return tokenizer.key(m_nHash); }
   bool empty() { return m_nHash==g_tokenForEmptyString; }
   bool unknown() { return m_nHash==g_tokenForUnknownString; }
   void clear() { m_nHash=g_tokenForEmptyString; }
}; 

/*===============================================================
 *
 * definitions about two word 
 *
 *==============================================================*/

#include "word_common.h"

inline unsigned long int hash(const CWord &w) {return w.hash();}

#endif

