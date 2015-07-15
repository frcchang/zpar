// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * word.h - the definitions for untokenized Chinese words       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _WORD_H
#define _WORD_H

#include "hash.h"

#include <std::string>

/*===============================================================
 *
 * definitions about word
 *
 *==============================================================*/

// the word class, which includes hash
class CWord {

protected:
   unsigned long int m_nHash;
   std::string m_sString;
   void operator += (const std::string &s) { for(int i=0; i<s.length(); ++i) (*this) += s[i]; }
   void operator += (const char &s) { m_sString += s; m_nHash = m_nHash * 37 + (static_cast<unsigned char>(s)&127); }

public:
   CWord() { clear(); }
   CWord(const std::string &s) { clear(); (*this)+=s; }
   CWord(const CWord &w) { m_nHash=w.m_nHash; m_sString = w.m_sString; }
   virtual ~CWord() {}

public:
   virtual unsigned long int hash() const { return m_nHash; }
   virtual bool operator == (const CWord &w) const { return (m_nHash == w.m_nHash) && (m_sString == w.m_sString); }
   virtual bool operator != (const CWord &w) const { return (m_nHash != w.m_nHash) || (m_sString != w.m_sString); }
   // the ordering of words are defined:
   // when the hash are not equal, order is defined by hash
   // when the hash are equal, order is defined by char-value
   virtual bool operator < (const CWord &w) const { return (m_nHash == w.m_nHash) ? (m_sString < w.m_sString) : (m_nHash < w.m_nHash); }
   void operator = (const std::string &s) { setString(s); }
   void setString(const std::string &s) { clear(); (*this) += s; }
   inline const std::string &str() const { return m_sString; }
   bool empty() { return m_sString.empty() ; }
   bool unknown() { return false; }
   bool clear() { m_sString = ""; m_nHash = 0; }
};

#include "word_common.h"

inline unsigned long int hash(const CWord &w) {return w.hash();}

#endif

