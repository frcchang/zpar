/****************************************************************
 *                                                              *
 * generic.h - the definitions for generic cfg tags with str    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_CFG_TAG_H
#define _GENERIC_CFG_TAG_H

#include "tokenizer.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * definitions about tag 
 *
 *==============================================================*/

class CConstituent {
protected:
   /*===============================================================
    *
    * the tokenizer
    *
    *==============================================================*/
   
   class CConstituentTokenizer : public CTokenizer<string, 251> {
      public: 
         CConstituentTokenizer() : CTokenizer<string, 251>(0/*reserve for NONE BEGIN END*/) {
            lookup("-NONE-");
            lookup("-BEGIN-");
         } 
         virtual ~CConstituentTokenizer() {}
   };

public:
   enum {NONE = 0};
   enum {SENTENCE_BEGIN = 1};
   enum {FIRST = 2};
   enum {SIZE = 8};
   static unsigned long COUNT;
   static unsigned long LAST;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CConstituentTokenizer &getTokenizer() const {static CConstituentTokenizer tokenizer; return tokenizer;}

public:
   CConstituent() : m_code(NONE) { }
   CConstituent(const CConstituent &t) { m_code=t.m_code; }
   CConstituent(const unsigned long &u) { assert(getTokenizer().count()>u); m_code=u; }
   CConstituent(const string &s) { load(s); }
   virtual ~CConstituent() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   const bool empty() const { return m_code==NONE; }
   bool operator == (const CConstituent &w) const { return m_code == w.m_code; }
   bool operator != (const CConstituent &w) const { return m_code != w.m_code; }
   bool operator < (const CConstituent &w) const { return m_code < w.m_code; }
   bool operator > (const CConstituent &w) const { return m_code > w.m_code; }
   bool operator <= (const CConstituent &w) const { return m_code <= w.m_code; }
   bool operator >= (const CConstituent &w) const { return m_code >= w.m_code; }

   void load(const string &s) {
      m_code=getTokenizer().lookup(s); 
      COUNT = getTokenizer().count();
      LAST = COUNT-1;
      assert((1<<SIZE)>COUNT);
   }
   void load(const unsigned long &code) {m_code=code;}
   const string &str() const { 
      return getTokenizer().key(m_code); 
   }

   inline bool canBeTemporary() const { return true; }
   // convenience optimization function 
   inline static bool canBeTemporary(const unsigned long &code) { return true; }
}; 

};

inline const unsigned long &hash(const TARGET_LANGUAGE::CConstituent &tag) {return tag.code(); }

#endif

