// Copyright (C) University of Oxford 2010
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

#ifndef GENERIC_CONLABEL_SIZE
#define GENERIC_CONLABEL_SIZE 8
#endif

#include "tokenizer.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * definitions about tag 
 *
 *==============================================================*/

class CConstituentLabel {
protected:
   /*===============================================================
    *
    * the tokenizer
    *
    *==============================================================*/
   
   class CConstituentLabelTokenizer : public CTokenizer<std::string, 251> {
      public: 
         CConstituentLabelTokenizer() : CTokenizer<std::string, 251>(0/*reserve for NONE BEGIN END*/) {
            lookup("-NONE-");
            lookup("-BEGIN-");
         } 
         virtual ~CConstituentLabelTokenizer() {}
   };

public:
   enum {NONE = 0};
   enum {SENTENCE_BEGIN = 1};
   enum {FIRST = 2};
   enum {SIZE = GENERIC_CONLABEL_SIZE};
   static unsigned long COUNT;
   static unsigned long LAST;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CConstituentLabelTokenizer &getTokenizer() const {static CConstituentLabelTokenizer tokenizer; return tokenizer;}

public:
   CConstituentLabel() : m_code(NONE) { }
   CConstituentLabel(const CConstituentLabel &t) { m_code=t.m_code; }
   CConstituentLabel(const unsigned long &u) { assert(getTokenizer().count()>u); m_code=u; }
   CConstituentLabel(const std::string &s) { load(s); }
   virtual ~CConstituentLabel() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   void copy(const CConstituentLabel &c) {m_code = c.m_code; }
   const void clear() { m_code=NONE; }
   const bool empty() const { return m_code==NONE; }
   bool operator == (const CConstituentLabel &w) const { return m_code == w.m_code; }
   bool operator != (const CConstituentLabel &w) const { return m_code != w.m_code; }
   bool operator < (const CConstituentLabel &w) const { return m_code < w.m_code; }
   bool operator > (const CConstituentLabel &w) const { return m_code > w.m_code; }
   bool operator <= (const CConstituentLabel &w) const { return m_code <= w.m_code; }
   bool operator >= (const CConstituentLabel &w) const { return m_code >= w.m_code; }

   void load(const std::string &s) {
      m_code=getTokenizer().lookup(s); 
//      if (getTokenizer().count()>COUNT) TRACE(s);
      COUNT = getTokenizer().count();
      LAST = COUNT-1;
      assert((1<<SIZE)>COUNT);
   }
   void load(const unsigned long &code) {m_code=code;}
   const std::string &str() const { 
      return getTokenizer().key(m_code); 
   }

   inline bool canBeTemporary() const { return true; }
   // convenience optimization function 
   inline static bool canBeTemporary(const unsigned long &code) { return true; }
}; 

};

inline const unsigned long &hash(const TARGET_LANGUAGE::CConstituentLabel &tag) {return tag.code(); }

#endif

