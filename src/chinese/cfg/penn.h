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
const string PENN_CON_STRINGS[] = {
   "-NONE-",
   "ADJP", "ADVP", 
   "CLP", "CP", 
   "DNP", "DP", "DVP", 
   "FRAG", 
   "IP", 
   "LCP", "LST", 
   "NP", 
   "PP", "PRN", 
   "QP", 
   "UCP", 
   "VCD", "VCP", "VNV", "VP", "VPT", "VRD", "VSB", 
   "WHNP", "WHPP"
};


enum PENN_CON_CONSTANTS {
   PENN_CON_NONE=0,
   PENN_CON_ADJP, PENN_CON_ADVP, 
   PENN_CON_CLP, PENN_CON_CP, 
   PENN_CON_DNP, PENN_CON_DP, PENN_CON_DVP, 
   PENN_CON_FRAG, 
   PENN_CON_IP, 
   PENN_CON_LCP , PENN_CON_LST, 
   PENN_CON_NP, 
   PENN_CON_PP, PENN_CON_PRN, 
   PENN_CON_QP, 
   PENN_CON_UCP, 
   PENN_CON_VCD, PENN_CON_VCP, PENN_CON_VNV, PENN_CON_VP, PENN_CON_VPT, PENN_CON_VRD, PENN_CON_VSB, 
   PENN_CON_WHNP, PENN_CON_WHPP,
   PENN_CON_COUNT
};

const bool PENN_CON_TEMP[] = {
   false,
   true, true, 
   false, true, 
   true, false, false, 
   true, 
   true, 
   true, false, 
   true, 
   true, true, 
   true, 
   true, 
   true, false, true, true, true, false, false, 
   false, false
};

const int PENN_CON_FIRST = 1;
const int PENN_CON_COUNT_BITS = 5; 

//===============================================================

class CConstituent {
public:
   enum {NONE=0};
   enum {COUNT=PENN_CON_COUNT};
   enum {SIZE=PENN_CON_COUNT_BITS};
   enum {FIRST=PENN_CON_FIRST};

protected:
   unsigned long m_code;

public:
   CConstituent() { m_code=PENN_CON_NONE; }
   CConstituent(PENN_CON_CONSTANTS t) { 
      m_code=t; 
   }
   CConstituent(const unsigned long &t) { 
      m_code=t; 
   }
   CConstituent(const string &s) { load(s); }
   virtual ~CConstituent() {}

public:
   const unsigned long &code() const { return m_code; }
   const unsigned long &hash() const { return m_code; }
   string str() const { assert(m_code<PENN_CON_COUNT) ; return PENN_CON_STRINGS[m_code]; }
   void load(const string &s) {
      m_code = PENN_CON_NONE ;
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
   void load(const unsigned long &code) {m_code=code;}
   inline bool canBeTemporary() const { return PENN_CON_TEMP[m_code]; }
   // convenience optimization function 
   inline static bool canBeTemporary(const unsigned long &code) { return PENN_CON_TEMP[code]; }

public:
   bool operator == (const CConstituent &t1) const { return m_code == t1.m_code; }
   bool operator < (const CConstituent &t1) const { return m_code < t1.m_code; }
   bool operator > (const CConstituent &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CConstituent &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CConstituent &t1) const { return m_code >= t1.m_code; }
};

};

