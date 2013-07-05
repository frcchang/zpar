// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * penn.h - the definitions for english cfg                     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

namespace english {

// the penn constituent set
const std::string PENN_CON_STRINGS[] = {
   "-NONE-",
   "-BEGIN-",
   "S", "SBAR", "SBARQ", "SINV", "SQ", 
   "ADJP", "ADVP", 
   "CONJP", 
   "FRAG", 
   "INTJ", 
   "LST", 
   "NAC", "NP", "NX", 
   "PP", "PRN", "PRT",
   "QP", 
   "RRC", 
   "UCP",
   "VP",
   "WHADJP", "WHADVP", "WHNP", "WHPP",
   "X"
};


enum PENN_CON_CONSTANTS {
   PENN_CON_NONE=0,
   PENN_CON_BEGIN=1,
   PENN_CON_S, PENN_CON_SBAR, PENN_CON_SBARQ, PENN_CON_SINV, PENN_CON_SQ, 
   PENN_CON_ADJP, PENN_CON_ADVP, 
   PENN_CON_CONJP, 
   PENN_CON_FRAG, 
   PENN_CON_INTJ, 
   PENN_CON_LST, 
   PENN_CON_NAC, PENN_CON_NP, PENN_CON_NX, 
   PENN_CON_PP, PENN_CON_PRN, PENN_CON_PRT,
   PENN_CON_QP, 
   PENN_CON_RRC, 
   PENN_CON_UCP,
   PENN_CON_VP,
   PENN_CON_WHADJP, PENN_CON_WHADVP, PENN_CON_WHNP, PENN_CON_WHPP, 
   PENN_CON_X, 
   PENN_CON_COUNT
};

const bool PENN_CON_TEMP[] = {
   false,
   false,
   true, true, true, true, true, 
   true, true, 
   true, 
   true, 
   true, 
   true, 
   true, true, true, 
   true, true, true,
   true, 
   true, 
   true,
   true,
   true, true, true, true, 
   true
};

const int PENN_CON_FIRST = 2;
const int PENN_CON_COUNT_BITS = 5; 

//===============================================================

class CConstituentLabel {
public:
   enum {NONE=0};
   enum {SENTENCE_BEGIN=PENN_CON_BEGIN};
   enum {FIRST=PENN_CON_FIRST};
   enum {COUNT=PENN_CON_COUNT}; 
   enum {SIZE=PENN_CON_COUNT_BITS};

protected:
   unsigned long m_code;

public:
   CConstituentLabel() { m_code=PENN_CON_NONE; }
   CConstituentLabel(PENN_CON_CONSTANTS t) { 
      m_code=t; 
   }
   CConstituentLabel(const unsigned long &t) { 
      m_code=t; 
   }
   CConstituentLabel(const std::string &s) { load(s); }
   virtual ~CConstituentLabel() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   void copy(const CConstituentLabel &c) {m_code = c.m_code; }
   const void clear() { m_code=NONE; }
   const bool empty() const { return m_code==NONE; }
   std::string str() const { assert(m_code<PENN_CON_COUNT) ; return PENN_CON_STRINGS[m_code]; }
   void load(const std::string &s) {
      m_code = PENN_CON_NONE ;
      bool bFound = false;
      if (s.empty()) return;
      for (int i=0; i<PENN_CON_COUNT; ++i) {
         if (PENN_CON_STRINGS[i] == s) {
            m_code = i;
            bFound = true;
         }
      }
      // unknown constituent
      if (!bFound) {
         THROW("unknown constituent: " << s << '.');
      }
   }
   void load(const unsigned long &code) {m_code=code;}
   inline bool canBeTemporary() const { return PENN_CON_TEMP[m_code]; }
   // convenience optimization function 
   inline static bool canBeTemporary(const unsigned long &code) { return PENN_CON_TEMP[code]; }

public:
   bool operator == (const CConstituentLabel &t1) const { return m_code == t1.m_code; }
   bool operator < (const CConstituentLabel &t1) const { return m_code < t1.m_code; }
   bool operator > (const CConstituentLabel &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CConstituentLabel &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CConstituentLabel &t1) const { return m_code >= t1.m_code; }
};

};

