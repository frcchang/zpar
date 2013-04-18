// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * ctb5.h - the definitions for chinese tags                    *
 *                                                              *
 * Author: meishan zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2013.01                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_TAG_PENN_H
#define _CHINESE_TAG_PENN_H

namespace chinese {

// the penn tag set
// Modify the following three constants together, keeping consistency!
const std::string PENN_TAG_STRINGS[] = {
   "-NONE-",
   "-BEGIN-",
   "-END-",
   "NN", "VV",
   "NR", "AD",
   "P", "CD", "M", "JJ",
   "DEC", "DEG",
   "NT", "CC", "VA", "LC",
   "PN", "DT", "VC", "AS", "VE",
   "OD", "IJ","ON",
   "ETC", "MSP", "CS", "BA",
   "DEV", "SB", "SP", "LB",
   "FW", "DER", "PU",
};


enum PENN_TAG_CONSTANTS {
   PENN_TAG_NONE=0,
   PENN_TAG_BEGIN,
   PENN_TAG_END,
   PENN_TAG_NN, PENN_TAG_VV,
   PENN_TAG_NR, PENN_TAG_AD,
   PENN_TAG_P, PENN_TAG_CD, PENN_TAG_M, PENN_TAG_JJ,
   PENN_TAG_DEC, PENN_TAG_DEG,
   PENN_TAG_NT, PENN_TAG_CC, PENN_TAG_VA, PENN_TAG_LC,
   PENN_TAG_PN, PENN_TAG_DT, PENN_TAG_VC, PENN_TAG_AS, PENN_TAG_VE,
   PENN_TAG_OD, PENN_TAG_IJ, PENN_TAG_ON,
   PENN_TAG_ETC, PENN_TAG_MSP, PENN_TAG_CS, PENN_TAG_BA,
   PENN_TAG_DEV, PENN_TAG_SB, PENN_TAG_SP, PENN_TAG_LB,
   PENN_TAG_FW, PENN_TAG_DER, PENN_TAG_PU,
   PENN_TAG_COUNT
};

const unsigned long PENN_TAG_FIRST = 3;
const unsigned long PENN_TAG_COUNT_BITS = 6; // 32 < bits < 64, takes 6 bits

const bool PENN_TAG_CLOSED[] = {
   false,
   false,
   false,
   false, false,
   false, false,
   true, false, false, false,
   true, true,
   false, true, false, true,
   true, true, true, true, true,
   false, true, true,
   true, true, true, true,
   true, true, true, true,
   false, true, true,
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

