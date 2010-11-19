// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * generic.h - the definitions for generic dependenby labels    *
 * with str;    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_DEPENDENCYLABEL_H
#define _GENERIC_DEPENDENCYLABEL_H

#include "tokenizer.h"

/*===============================================================
 *
 * definitions abstd::cout tag 
 *
 *==============================================================*/

namespace generic {
class CDependencyLabel {
private:
   /*===============================================================
    *
    * the tokenizer
    *
    *==============================================================*/
   
   class CDependencyLabelTokenizer : public CTokenizer<std::string, 251> {
      public: 
         CDependencyLabelTokenizer() : CTokenizer<std::string, 251>(0/*reserve for NONE BEGIN END*/) {
            lookup("-NONE-");
            lookup("-ROOT-");
         } 
         virtual ~CDependencyLabelTokenizer() {}
   };

public:
   enum {NONE = 0};
   enum {ROOT = 1};
   enum {FIRST = 1};
   enum {SIZE = 8};
   enum {MAX_COUNT = 1<<SIZE};
   static unsigned long COUNT;
   static unsigned long LAST;

protected:
   unsigned long m_code;

protected:
   // static method assigns tokenizer as global dictionary of words
   CDependencyLabelTokenizer &getTokenizer() const {static CDependencyLabelTokenizer tokenizer; return tokenizer;}

public:
   CDependencyLabel() : m_code(NONE) { }
   CDependencyLabel(const std::string &s) { load(s); }
   CDependencyLabel(const CDependencyLabel &t) : m_code(t.m_code) { }
   CDependencyLabel(const unsigned &u) { assert(getTokenizer().count()>u); m_code=u; }
   virtual ~CDependencyLabel() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   bool operator == (const CDependencyLabel &l) const { return m_code == l.m_code; }
   bool operator != (const CDependencyLabel &l) const { return m_code != l.m_code; }
   bool operator < (const CDependencyLabel &l) const { return m_code < l.m_code; }
   bool operator > (const CDependencyLabel &l) const { return m_code > l.m_code; }
   bool operator <= (const CDependencyLabel &l) const { return m_code <= l.m_code; }
   bool operator >= (const CDependencyLabel &l) const { return m_code >= l.m_code; }

   void load(const std::string &s) {
      m_code=getTokenizer().lookup(s); 
      COUNT = getTokenizer().count();
      LAST = COUNT-1;
      assert((1<<SIZE)>COUNT);
   }
   void load(const unsigned long &u) {
      m_code = u;
   }
   const std::string &str() const { 
      return getTokenizer().key(m_code); 
   }

   void clear() { m_code=NONE; }
}; 
inline std::istream & operator >> (std::istream &is, CDependencyLabel &label) {
   std::string s;
   is >> s;
   label.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const CDependencyLabel &label) {
   os << label.str() ;
   return os;
}

} // namespace generic

#endif
