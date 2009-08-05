/****************************************************************
 *                                                              *
 * cfg.h - the definitions for chinese cfg                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_CFG_H
#define _CHINESE_CFG_H 1

#include "definitions.h"

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
const int PENN_CON_TAG_COUNT_BITS = 7; // MAX(PENN_CON_COUNT_BITS, PENN_TAG_COUNT_BITS) + 1

//===============================================================

class CConstituent {
protected:
   unsigned int m_code;

public:
   CConstituent() { m_code=PENN_CON_NONE; }
   CConstituent(PENN_CON_CONSTANTS t) { 
      m_code=t; 
   }
   CConstituent(int t) { 
      m_code=t; 
   }
   CConstituent(const string &s, bool bNotNone=false) { load(s, bNotNone); }
   virtual ~CConstituent() {}

public:
   unsigned int code() const { return m_code; }
   string str() const { assert(m_code<PENN_CON_COUNT) ; return PENN_CON_STRINGS[m_code]; }
   void load(const string &s, bool bNoneDefault) {
      m_code = PENN_CON_NONE ;
      bool bFound = false;
      for (int i=0; i<PENN_CON_COUNT; i++) {
         if (PENN_CON_STRINGS[i] == s) {
            m_code = i;
            bFound = true;
         }
      }
      // unknown constituent
      if (bNoneDefault && !bFound) {
         ostringstream os; os << "unknown constituent: " << s << '.';
         throw(os.str());
      }
   }

public:
   bool operator == (const CConstituent &t1) const { return m_code == t1.m_code; }
   bool operator < (const CConstituent &t1) const { return m_code < t1.m_code; }
   bool operator > (const CConstituent &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CConstituent &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CConstituent &t1) const { return m_code >= t1.m_code; }
};

//===============================================================

inline unsigned int encodeConstituents(const unsigned &con1, const unsigned &con2) {
   return (con1<<PENN_CON_COUNT_BITS) | con2;
}
inline unsigned int encodeConstituents(const unsigned &con1, const unsigned &con2, const unsigned &con3) {
   return (con1<<PENN_CON_COUNT_BITS*2) | (con2<<PENN_CON_COUNT_BITS) | con3 ;
}
inline unsigned int encodeConstituents(const unsigned &con1, const unsigned &con2, const unsigned &con3, const unsigned &con4) {
   return (con1<<PENN_CON_COUNT_BITS*3) | (con2<<PENN_CON_COUNT_BITS*2) |
          (con3<<PENN_CON_COUNT_BITS) | con4 ;
}

inline unsigned int encodeConstituents(const CConstituent &con1, const CConstituent &con2) {
   return (con1.code()<<PENN_CON_COUNT_BITS) | con2.code();
}
inline unsigned int encodeConstituents(const CConstituent &con1, const CConstituent &con2, const CConstituent &con3) {
   return (con1.code()<<PENN_CON_COUNT_BITS*2) | (con2.code()<<PENN_CON_COUNT_BITS) | con3.code() ;
}
inline unsigned int encodeConstituents(const CConstituent &con1, const CConstituent &con2, const CConstituent &con3, const CConstituent &con4) {
   return (con1.code()<<PENN_CON_COUNT_BITS*3) | (con2.code()<<PENN_CON_COUNT_BITS*2) |
          (con3.code()<<PENN_CON_COUNT_BITS) | con4.code() ;
}

}; // namespace chinese

//===============================================================

inline unsigned long hash(const chinese::CConstituent &con) { return con.code();}

//===============================================================

inline istream & operator >> (istream &is, chinese::CConstituent &con) {
   string s;
   is >> s;
   con.load(s, false);
   return is;
}

inline ostream & operator << (ostream &os, const chinese::CConstituent &con) {
   os << con.str() ;
   return os;
}

#endif

