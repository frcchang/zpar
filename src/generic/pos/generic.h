// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * generictag.h - the definitions for generic tags with str;    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_POS_H
#define _GENERIC_POS_H

#include "tokenizer.h"

namespace generic {

const char TAG_SEPARATOR = '/';

/*===============================================================
 *
 * definitions about tag 
 *
 *==============================================================*/

class CTag {

   /*===============================================================
    *
    * the tokenizer
    *
    *==============================================================*/
   
   class CTagTokenizer : public CTokenizer<std::string, 251> {
      public: 
         CTagTokenizer() : CTokenizer<std::string, 251>(0/*reserve for NONE BEGIN END*/) {
            lookup("-NONE-");
            lookup("-BEGIN-");
            lookup("-END-");
         } 
         virtual ~CTagTokenizer() {}
   };

public:
   enum {NONE = 0};
   enum {SENTENCE_BEGIN = 1};
   enum {SENTENCE_END = 2};
   enum {FIRST = 3};
   enum {SIZE = 8};
   static unsigned long COUNT;
   static unsigned long LAST;
   static const unsigned long MAX_COUNT=1<<SIZE;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CTagTokenizer &getTokenizer() const { 
      // avoid static initialization-fiasco
      static CTagTokenizer tokenizer;
      return tokenizer; 
   }

public:
   CTag() : m_code(NONE) { }
   CTag(const std::string &s) { load(s); }
   CTag(const CTag &t) : m_code(t.m_code) { }
   CTag(const unsigned long &u) { assert(getTokenizer().count()>u); m_code=u; }
   virtual ~CTag() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   void copy(const CTag &t) { m_code = t.m_code; }
   bool operator == (const CTag &w) const { return m_code == w.m_code; }
   bool operator != (const CTag &w) const { return m_code != w.m_code; }
   bool operator < (const CTag &w) const { return m_code < w.m_code; }
   bool operator > (const CTag &w) const { return m_code > w.m_code; }
   bool operator <= (const CTag &w) const { return m_code <= w.m_code; }
   bool operator >= (const CTag &w) const { return m_code >= w.m_code; }
   void operator = (const CTag &a) { m_code = a.m_code; }

   void load(const std::string &s) {
      m_code=getTokenizer().lookup(s); 
      COUNT = getTokenizer().count();
      LAST = COUNT-1;
      ASSERT((1<<SIZE)>COUNT, "The number of POS tags larger than the generic tag max size: change generic pos.");
   }
   void load(const unsigned &i) {
      assert(i<COUNT);
      m_code = i;
   }
   const std::string &str() const { 
      return getTokenizer().key(m_code); 
   }

   void clear() { m_code=NONE; }
   bool closed() const { return false; }
}; 

};

inline const unsigned long &hash(const generic::CTag &tag) {return tag.code(); }

//===============================================================

inline std::istream & operator >> (std::istream &is, generic::CTag &tag) {
   std::string s;
   is >> s;
   tag.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const generic::CTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

