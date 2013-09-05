/**
 * Spanish dependency labels from the CoNLL 2006 (CoNLL-X) label set
 *
 * Carlos Gomez-Rodriguez, Yue Zhang
 */

#ifndef _DEPENDENCY_ES_CONLL06
#define _DEPENDENCY_ES_CONLL06

#include "tags.h"

namespace spanish {

/*
 * Labels have been extracted directly from the CoNLL treebank (plus the special label -NONE-).
 * Note that not all the labels described in the cast3lb treebank documentation actually appear
 * in the CoNLL treebank. Those not appearing are not listed here.
 */

const std::string ES06_DEP_STRINGS[] = {
		"-NONE-",
		"ROOT",
		"ATR",
		"ATR.d",
		"CAG",
		"CC",
		"CD",
		"CD.Q",
		"CI",
		"CPRED",
		"CPRED.CD",
		"CPRED.SUJ",
		"CREG",
		"ET",
		"IMPERS",
		"MOD",
		"NEG",
		"PASS",
		"PUNC",
		"SUJ",
		"VOC",
		"_" //most dependencies have this label in the CoNLL-X dataset
};

enum ES06_DEP_LABELS {
	ES06_DEP_NONE = 0,
	ES06_DEP_ROOT,
	ES06_DEP_ATR,
	ES06_DEP_ATR_D,
	ES06_DEP_CAG,
	ES06_DEP_CC,
	ES06_DEP_CD,
	ES06_DEP_CD_Q,
	ES06_DEP_CI,
	ES06_DEP_CPRED,
	ES06_DEP_CPRED_CD,
	ES06_DEP_CPRED_SUJ,
	ES06_DEP_CREG,
	ES06_DEP_ET,
	ES06_DEP_IMPERS,
	ES06_DEP_MOD,
	ES06_DEP_NEG,
	ES06_DEP_PASS,
	ES06_DEP_PUNC,
	ES06_DEP_SUJ,
	ES06_DEP_VOC,
	ES06_DEP_UNSPECIFIED,

	ES06_DEP_COUNT
};

const unsigned long ES06_DEP_COUNT_BITS = 5;

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
   enum {COUNT=ES06_DEP_COUNT};
   enum {MAX_COUNT=COUNT};
   enum {SIZE=ES06_DEP_COUNT_BITS};

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
      m_code = ES06_DEP_NONE;
      for (int i=FIRST; i<COUNT; ++i) {
         if (ES06_DEP_STRINGS[i]==str) {
            m_code = i;
            return;
         }
      }
   }

   void load(const unsigned long &u) { 
      m_code = u;
   }

   const std::string &str() const { 
      return ES06_DEP_STRINGS[ m_code ];
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
