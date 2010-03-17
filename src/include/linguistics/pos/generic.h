/****************************************************************
 *                                                              *
 * generictag.h - the definitions for generic tags with str;    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_TAG_H
#define _GENERIC_TAG_H

#include "tokenizer.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * the tokenizer
 *
 *==============================================================*/

class CTagTokenizer : public CTokenizer<string, 251> {
   public: 
      CTagTokenizer() : CTokenizer<string, 251>(0/*reserve for NONE BEGIN END*/) {
         lookup("-NONE-");
         lookup("-BEGIN-");
         lookup("-END-");
      } 
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
   enum {SIZE = 8};
   static unsigned long COUNT;
   static unsigned long LAST;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CTagTokenizer &getTokenizer() const {static CTagTokenizer tokenizer; return tokenizer;}

public:
   CTag() { clear(); }
   CTag(const string &s) { load(s); }
   CTag(const CTag &t) { m_code=t.m_code; }
   CTag(const unsigned &u) { assert(getTokenizer().count()>u); m_code=u; }
   virtual ~CTag() {}

public:
   const unsigned long &code() const { return m_code; }
   bool operator == (const CTag &w) const { return m_code == w.m_code; }
   bool operator != (const CTag &w) const { return m_code != w.m_code; }
   bool operator < (const CTag &w) const { return m_code < w.m_code; }
   bool operator > (const CTag &w) const { return m_code > w.m_code; }
   bool operator <= (const CTag &w) const { return m_code <= w.m_code; }
   bool operator >= (const CTag &w) const { return m_code >= w.m_code; }

   void load(const string &s) {
      m_code=getTokenizer().lookup(s); 
      COUNT = getTokenizer().count();
      LAST = COUNT-1;
      assert((1<<SIZE)>COUNT);
   }
   const string &str() const { 
      return getTokenizer().key(m_code); 
   }

   void clear() { m_code=NONE; }
}; 

inline const unsigned long &hash(const CTag &tag) {return tag.code(); }

};

inline const unsigned long &hash(const TARGET_LANGUAGE::CTag &tag) {return tag.code(); }

#endif

