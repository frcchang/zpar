// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * generictag.h - the definitions for generic tags with str;    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_TAG_H
#define _GENERIC_TAG_H

#include "tokenizer.h"

/*===============================================================
 *
 * the tagset
 *
 *==============================================================*/

class CGenericTagset : public CTokenizer<std::string, 251> {
   public: 
      CGenericTagset() : CTokenizer<std::string, 251>(0) {
         lookup("-NONE-");
      } 
      virtual ~CGenericTagset() {}
};

/*===============================================================
 *
 * definitions about tag 
 *
 * Each tag is associated with a tagset. 
 * This is different from the concept of POS tags, which are 
 * assocaited with a universal set. When a program runs, the
 * set of POS tag for each language is fixed. 
 * For a generic tag, de to its generic purpose, there can 
 * be differnt sets of tags coexistance. An example is CoNLL CTag/CFeats.
 *
 *==============================================================*/

class CGenericTag {
public:
   enum {NONE = 0};
   enum {FIRST = 1};

protected:
   unsigned long m_code;

public:
   CGenericTag() : m_code(NONE) { }
   CGenericTag(const std::string &s) { load(s); }
   CGenericTag(const CGenericTag &t) : m_code(t.m_code) { }
   CGenericTag(const unsigned long &u) : m_code(u) { }
   virtual ~CGenericTag() {}

public:
   virtual CGenericTagset &getTagset() const = 0;

public:
   const unsigned long &code() const { 
      return m_code; 
   }
   const unsigned long &hash() const {
      return m_code; 
   }
   void copy(const CGenericTag &t) { 
      m_code = t.m_code; 
   }
   void operator = (const CGenericTag &w) { 
      m_code = w.m_code; 
   }
   bool operator == (const CGenericTag &w) const { 
      return m_code == w.m_code; 
   }
   bool operator != (const CGenericTag &w) const { 
      return ! ((*this)==w) ; 
   }
   bool operator < (const CGenericTag &w) const { 
      return m_code < w.m_code; 
   }
   bool operator > (const CGenericTag &w) const { 
      return m_code > w.m_code; 
   }
   bool operator <= (const CGenericTag &w) const { 
      return m_code <= w.m_code; 
   }
   bool operator >= (const CGenericTag &w) const { 
      return m_code >= w.m_code; 
   }

   void load(const std::string &s) {
      if (s.empty()) 
         m_code = NONE; // empty are equal to NONE
      else
         m_code=getTagset().lookup(s); 
   }
   const std::string &str() const { 
      return getTagset().key(m_code); 
   }

   void clear() { 
      m_code=NONE; 
   }
}; 

//===============================================================

inline std::istream & operator >> (std::istream &is, CGenericTag &tag) {
   std::string s;
   is >> s;
   tag.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const CGenericTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

