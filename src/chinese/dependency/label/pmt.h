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

const std::string PENN_DEP_STRINGS[] = {
   "-NONE-",
   "ROOT",
   "HED", 
   "ACT",
   "ADV",
   "APP",
   "ATT",
   "CMP",
   "COO",
   "COS",
   "DE",
   "DEI",
   "DI",
   "FOC",
   "IC",
   "IOB",
   "IS",
   "ISC",
   "LAD",
   "MT",
   "NUM",
   "POB",
   "PUN",
   "PUS",
   "QUC",
   "QUN",
   "RAD",
   "RED",
   "SBV",
   "TPC",
   "VOB",
   "RADC",
   "QUCC",
   "VV"

};

enum PENN_DEP_LABELS {
   PENN_DEP_NONE=0,
   PENN_DEP_ROOT, 
   PENN_DEP_HED,
   PENN_DEP_ACT,
   PENN_DEP_ADV,
   PENN_DEP_APP,
   PENN_DEP_ATT,
   PENN_DEP_CMP,
   PENN_DEP_COO,
   PENN_DEP_COS,
   PENN_DEP_DE,
   PENN_DEP_DEI,
   PENN_DEP_DI,
   PENN_DEP_FOC,
   PENN_DEP_IC,
   PENN_DEP_IOB,
   PENN_DEP_IS,
   PENN_DEP_ISC,
   PENN_DEP_LAD,
   PENN_DEP_MT,
   PENN_DEP_NUM,
   PENN_DEP_POB,
   PENN_DEP_PUN,
   PENN_DEP_PUS,
   PENN_DEP_QUC,
   PENN_DEP_QUN,
   PENN_DEP_RAD,
   PENN_DEP_RED,
   PENN_DEP_SBV,
   PENN_DEP_TPC,
   PENN_DEP_VOB,
   PENN_DEP_RADC,
   PENN_DEP_QUCC,
   PENN_DEP_VV, 
   PENN_DEP_COUNT
};

const unsigned long PENN_DEP_COUNT_BITS = 5;

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
