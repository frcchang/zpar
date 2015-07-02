// Copyright (C) University of Oxford 2014
/****************************************************************
 *                                                              *
 * pmt.h - the definitions for chinese tags in the PMT Treebank *
 *                                                              *
 * Author: Likun Qiu                                            *
 *                                                              *
 * Computing Laboratory, SUTD. 2014  .01                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_TAG_PENN_H
#define _CHINESE_TAG_PENN_H

namespace chinese {

// the pmt tag set
// Modify the following three constants together, keeping consistency!
const std::string PENN_TAG_STRINGS[] = {
   "-NONE-",
   "-BEGIN-",
   "-END-",
   "n","v","w","u",
   "m","p","c","f",
   "r","d","a","b",
   "q","t","ns","s",
   "z","nz","y","nr","nrf","nrg",
   "nt","nx","vn","an",
   "e","o","l","i","k","h","x","j"
};


enum PENN_TAG_CONSTANTS {
   PENN_TAG_NONE=0,
   PENN_TAG_BEGIN,
   PENN_TAG_END,
   PENN_TAG_N, PENN_TAG_V,PENN_TAG_W, PENN_TAG_U,
   PENN_TAG_M, PENN_TAG_P,PENN_TAG_C, PENN_TAG_F,
   PENN_TAG_R, PENN_TAG_D, PENN_TAG_A, PENN_TAG_B,
   PENN_TAG_Q, PENN_TAG_T,PENN_TAG_NS, PENN_TAG_S,
   PENN_TAG_Z, PENN_TAG_NZ, PENN_TAG_Y, PENN_TAG_NR, PENN_TAG_NRF, PENN_TAG_NRG,
   PENN_TAG_NT, PENN_TAG_NX, PENN_TAG_VN, PENN_TAG_AN, 
   PENN_TAG_E,  PENN_TAG_O,  PENN_TAG_L,  PENN_TAG_I,  PENN_TAG_K,  PENN_TAG_H,  PENN_TAG_X,  PENN_TAG_J, 
   PENN_TAG_COUNT
};

const int PENN_TAG_FIRST = 3;
const int PENN_TAG_COUNT_BITS = 6; // 32 < bits < 64, takes 6 bits

const bool PENN_TAG_CLOSED[] = {
   false,
   false,
   false,
   false, false, true, true,
   false, true, true, true,
   true, false, false, false,
   true, false, false, false,
   false, false, true, false,false,false,
   false, false, false, false,
   true, true, true, true,true,true,false,false
};

//===============================================================

class CTag {
public:
   enum {SENTENCE_BEGIN = PENN_TAG_BEGIN};
   enum {SENTENCE_END = PENN_TAG_END};
   enum {COUNT = PENN_TAG_COUNT};
   enum {MAX_COUNT = PENN_TAG_COUNT};
   enum {NONE = PENN_TAG_NONE};
   enum {SIZE = PENN_TAG_COUNT_BITS};
   enum {FIRST = PENN_TAG_FIRST};
   enum {LAST = PENN_TAG_COUNT-1};

protected:
   unsigned long m_code;

public:
   CTag() : m_code(NONE) {}
   CTag(PENN_TAG_CONSTANTS t) : m_code(t) { }
   CTag(int t) : m_code(t) { assert(t<PENN_TAG_COUNT); }
   CTag(const std::string &s) { load(s); }
   virtual ~CTag() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   std::string str() const {
      assert(m_code<(1<<PENN_TAG_COUNT_BITS)) ;
      if (m_code>=PENN_TAG_COUNT) {
         std::stringstream ss;
         ss << "EXTRA(";
         ss << m_code;
         ss << ")";
         return ss.str();
      }
      return PENN_TAG_STRINGS[m_code];
   }
   void load(const std::string &s) {
      //m_code = PENN_TAG_NONE ;
      for (int i=0; i<PENN_TAG_COUNT; ++i)
         if (PENN_TAG_STRINGS[i] == s) {
            m_code = i;
            return;
         }
      THROW("Unknown tag (PENN Treebank): " << s);
   }
   bool loadcheck(const std::string &s) {
      //m_code = PENN_TAG_NONE ;
      for (int i=0; i<PENN_TAG_COUNT; ++i)
         if (PENN_TAG_STRINGS[i] == s) {
            m_code = i;
            return true;
         }
      m_code = 0;
      return false;
   }
   void load(const unsigned long &n) { m_code = n; }
   void copy(const CTag &t) { m_code = t.m_code; }

public:
   bool operator == (const CTag &t1) const { return m_code == t1.m_code; }
   bool operator != (const CTag &t1) const { return m_code != t1.m_code; }
   bool operator < (const CTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CTag &t1) const { return m_code >= t1.m_code; }
};

}; // namespace chinese

#endif

