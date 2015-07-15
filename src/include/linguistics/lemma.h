// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * lemma.h - the definitions for lemma                          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _LEMMA_H
#define _LEMMA_H

#include "tokenizer.h"

/*===============================================================
 *
 * definitions about lemma
 *
 *==============================================================*/

class CLemma {

private:
   /*===============================================================
    *
    * the tokenizer
    *
    *==============================================================*/

   class CStringTokenizer : public CTokenizer<std::string, 65537> {
      public:
         CStringTokenizer() : CTokenizer<std::string, 65537>(1/*reserve for the other unknown */) {lookup("");}
         virtual ~CStringTokenizer() {}
   };

protected:
   unsigned long m_nHash;

protected:
   // static method assigns tokenizer as global dictionary of words
   CStringTokenizer &getTokenizer() const {static CStringTokenizer tokenizer; return tokenizer;}

public:
   enum {NONE=0, EMPTY=1};

public:
   CLemma() { clear(); }
   CLemma(const std::string &s, bool bModify=true) : m_nHash(bModify ?  getTokenizer().lookup(s) : getTokenizer().find(s, NONE)) { }
   CLemma(const CLemma &w) : m_nHash(w.m_nHash) { }
   CLemma(const unsigned long &n) : m_nHash(n) { }
   virtual ~CLemma() {}

public:
   unsigned long hash() const { return m_nHash; }
   unsigned long code() const { return m_nHash; }
   bool operator == (const CLemma &w) const { return m_nHash == w.m_nHash; }
   bool operator != (const CLemma &w) const { return m_nHash != w.m_nHash; }
   bool operator < (const CLemma &w) const { return m_nHash < w.m_nHash; }
   void operator = (const CLemma &w) { m_nHash =  w.m_nHash; }
   void copy(const CLemma &w) { m_nHash = w.m_nHash; }
   void setString(const std::string &s) { m_nHash = getTokenizer().find(s, NONE); }
   // do not use str() for unknown lemmas!!
   const std::string &str() const { ASSERT(m_nHash<getTokenizer().count(), "lemma.h: str() called for an unknown lemma"); return getTokenizer().key(m_nHash); }
   void load(const std::string &s) { m_nHash = getTokenizer().lookup(s); }
   bool empty() { return m_nHash==EMPTY; }
   bool none() { return m_nHash==NONE; }
   void clear() { m_nHash=EMPTY; }
};

//===============================================================

inline std::istream & operator >> (std::istream &is, CLemma &w) {
   std::string s ;
   is >> s ;
   ASSERT( s[0]=='[' && s[s.size()-1] == ']' ,  "Assertion failed in lemma.h, operator >> for class CLemma: This might be due to a malformed lemma (e.g. containing a space character) in the training set" ) ; // [] avoids empty
   w = s.substr(1,s.size()-2) ;
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CLemma &w) {
   os << '[' << w.str() << ']' ;
   return os ;
}

#endif

