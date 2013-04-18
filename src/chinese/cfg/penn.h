// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * penn.h - the definitions for penn cfg                        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2010.03                        *
 *                                                              *
 ****************************************************************/

namespace chinese {

// the penn constituent set
const std::string PENN_CON_STRINGS[] = {
   "-NONE-",
   "-BEGIN-",
   "ADJP", "ADVP", 
   "CLP", "CP", 
   "DNP", "DP", "DVP", 
   "FRAG", 
   "IP", 
   "INTJ",
   "FLR",
   "LCP", "LST", 
   "NP", 
   "PP", "PRN", 
   "QP", 
   "UCP", 
   "VCD", "VCP", "VNV", "VP", "VPT", "VRD", "VSB", 
   "WHNP", "WHPP", "AA", "INC", "DFL"
};


enum PENN_CON_CONSTANTS {
   PENN_CON_NONE=0,
   PENN_CON_BEGIN,
   PENN_CON_ADJP, PENN_CON_ADVP, 
   PENN_CON_CLP, PENN_CON_CP, 
   PENN_CON_DNP, PENN_CON_DP, PENN_CON_DVP, 
   PENN_CON_FRAG, 
   PENN_CON_IP, 
   PENN_CON_INTJ,
   PENN_CON_FLR,
   PENN_CON_LCP , PENN_CON_LST, 
   PENN_CON_NP, 
   PENN_CON_PP, PENN_CON_PRN, 
   PENN_CON_QP, 
   PENN_CON_UCP, 
   PENN_CON_VCD, PENN_CON_VCP, PENN_CON_VNV, PENN_CON_VP, PENN_CON_VPT, PENN_CON_VRD, PENN_CON_VSB, 
   PENN_CON_WHNP, PENN_CON_WHPP, PENN_CON_AA, PENN_CON_INC, PENN_CON_DFL,
   PENN_CON_COUNT
};

const bool PENN_CON_TEMP[] = {
   false,
   false,
   true, true, 
   false, true, 
   true, false, false, 
   true, 
   true, 
   true,
   false,
   true, false, 
   true, 
   true, true, 
   true, 
   true, 
   true, false, true, true, true, false, false, 
   false, false, false, true, true
};

const int PENN_CON_FIRST = 2;
const int PENN_CON_COUNT_BITS = 6;

//===============================================================

class CConstituentLabel {
public:
   enum {NONE=0};
   enum {SENTENCE_BEGIN=1};
   enum {COUNT=PENN_CON_COUNT};
   enum {SIZE=PENN_CON_COUNT_BITS};
   enum {FIRST=PENN_CON_FIRST};

protected:
   unsigned long m_code;

public:
   CConstituentLabel() { m_code=PENN_CON_NONE; }
   CConstituentLabel(const unsigned long &t) { 
      assert(t<COUNT);
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
      if (s.empty()) return;
      bool bFound = false;
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
   void load(const unsigned long &code) { assert(code<COUNT); m_code=code; }
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

