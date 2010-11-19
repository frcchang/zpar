// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * penn.h - the penn treebank style dependency labels           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.07                        *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_L_PENN
#define _DEPENDENCY_L_PENN

#include "tags.h"

namespace english {

const std::string PENN_DEP_STRINGS[] = {
   "-NONE-",
   "ROOT", 
   "AMOD",
   "DEP", 
   "NMOD", 
   "OBJ", 
   "P",
   "PMOD", 
   "PRD",
   "SBAR",
   "SUB",
   "VC",
   "VMOD"
};

enum PENN_DEP_LABELS {
   PENN_DEP_NONE=0,
   PENN_DEP_ROOT, 
   PENN_DEP_AMOD, 
   PENN_DEP_DEP, 
   PENN_DEP_NMOD, 
   PENN_DEP_OBJ, 
   PENN_DEP_P, 
   PENN_DEP_PMOD, 
   PENN_DEP_PRD, 
   PENN_DEP_SBAR, 
   PENN_DEP_SUB, 
   PENN_DEP_VC, 
   PENN_DEP_VMOD, 
   PENN_DEP_COUNT 
};

const unsigned long PENN_DEP_COUNT_BITS = 4;

/*==============================================================
 *
 * dependency lab
 *
 *==============================================================*/

class CDependencyLabel {

public:

   enum {NONE=0};
   enum {ROOT=1};
   enum {FIRST=1};
   enum {COUNT=PENN_DEP_COUNT};
   enum {MAX_COUNT=COUNT};
   enum {SIZE=PENN_DEP_COUNT_BITS};

protected:

   unsigned long m_code;

public:

   CDependencyLabel() : m_code(NONE) {}
   CDependencyLabel(const unsigned long &code) : m_code(code) { }
   CDependencyLabel(const std::string &str) { load(str); }
   virtual ~CDependencyLabel() {}

public:

   const unsigned long &hash() const { return m_code; }
   bool operator == (const CDependencyLabel &l) const { return m_code == l.m_code; }
   bool operator != (const CDependencyLabel &l) const { return m_code != l.m_code; }
   bool operator < (const CDependencyLabel &l) const { return m_code < l.m_code; }
   bool operator > (const CDependencyLabel &l) const { return m_code > l.m_code; }
   bool operator <= (const CDependencyLabel &l) const { return m_code <= l.m_code; }
   bool operator >= (const CDependencyLabel &l) const { return m_code >= l.m_code; }

   void load(const std::string &str) { 
      m_code = PENN_DEP_NONE;
      for (int i=FIRST; i<COUNT; ++i) {
         if (PENN_DEP_STRINGS[i]==str) {
            m_code = i;
            return;
         }
      }
   }

   void load(const unsigned long &u) { 
      m_code = u;
   }

   const std::string &str() const { 
      return PENN_DEP_STRINGS[ m_code ]; 
   }

   const unsigned long &code() const {
      return m_code;
   }

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
};

#endif
