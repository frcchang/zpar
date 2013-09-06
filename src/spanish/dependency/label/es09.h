/**
 * Spanish dependency labels from the AnCoRa 2.0 (and CoNLL 2009) label set.
 *
 * Carlos Gomez-Rodriguez, Yue Zhang
 */

#ifndef _DEPENDENCY_ES_CONLL09
#define _DEPENDENCY_ES_CONLL09

#include "tags.h"

namespace spanish {

/*
 * Labels have been extracted directly from the CoNLL treebank (plus the special label -NONE-).
 */

const std::string ES09_DEP_STRINGS[] = {
		"-NONE-",

		"ROOT",

		"S",
		"a",
		"ao",
		"atr",
		"c",
		"cag",
		"cc",
		"cd",
		"ci",
		"conj",
		"coord",
		"cpred",
		"creg",
		"d",
		"et",
		"f",
		"gerundi",
		"grup.a",
		"grup.adv",
		"grup.nom",
		"grup.verb",
		"i",
		"impers",
		"inc",
		"infinitiu",
		"interjeccio",
		"mod",
		"morfema.pronominal",
		"morfema.verbal",
		"n",
		"neg",
		"p",
		"participi",
		"pass",
		"prep",
		"r",
		"relatiu",
		"s",
		"s.a",
		"sa",
		"sadv",
		"sentence",
		"sn",
		"sp",
		"spec",
		"suj",
		"v",
		"voc",
		"w",
		"z",

		"_" //most dependencies have this label in the CoNLL-X dataset
};

enum ES09_DEP_LABELS {
	ES09_DEP_NONE = 0,

	ES09_DEP_ROOT,

	ES09_DEP_S,
	ES09_DEP_A,
	ES09_DEP_AO,
	ES09_DEP_ATR,
	ES09_DEP_C,
	ES09_DEP_C_AGENT,
	ES09_DEP_C_CIRC,
	ES09_DEP_C_DIRECT,
	ES09_DEP_C_INDIRECT,
	ES09_DEP_CONJ,
	ES09_DEP_COORD,
	ES09_DEP_C_PRED,
	ES09_DEP_C_REGIME,
	ES09_DEP_DET,
	ES09_DEP_ET,
	ES09_DEP_PUNCT,
	ES09_DEP_GERUND,
	ES09_DEP_GROUP_A,
	ES09_DEP_GROUP_ADV,
	ES09_DEP_GROUP_NOM,
	ES09_DEP_GROUP_VERB,
	ES09_DEP_I,
	ES09_DEP_IMPERS,
	ES09_DEP_INC,
	ES09_DEP_INFINITIVE,
	ES09_DEP_INTERJ,
	ES09_DEP_MOD,
	ES09_DEP_MORPH_PRON,
	ES09_DEP_MORPH_VERB,
	ES09_DEP_N,
	ES09_DEP_NEG,
	ES09_DEP_P,
	ES09_DEP_PARTICIP,
	ES09_DEP_PASS,
	ES09_DEP_PREP,
	ES09_DEP_R,
	ES09_DEP_RELAT,
	ES09_DEP_LOWERCASE_S,
	ES09_DEP_S_A,
	ES09_DEP_SA,
	ES09_DEP_SADV,
	ES09_DEP_SENTENCE,
	ES09_DEP_SYNT_NOM,
	ES09_DEP_SYNT_PREP,
	ES09_DEP_SPEC,
	ES09_DEP_SUBJ,
	ES09_DEP_V,
	ES09_DEP_VOC,
	ES09_DEP_W,
	ES09_DEP_Z,

	ES09_DEP_UNSPECIFIED,

	ES09_DEP_COUNT
};

const unsigned long ES09_DEP_COUNT_BITS = 6;

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
   enum {COUNT=ES09_DEP_COUNT};
   enum {MAX_COUNT=COUNT};
   enum {SIZE=ES09_DEP_COUNT_BITS};

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
      m_code = ES09_DEP_NONE;
      for (int i=FIRST; i<COUNT; ++i) {
         if (ES09_DEP_STRINGS[i]==str) {
            m_code = i;
            return;
         }
      }
   }

   void load(const unsigned long &u) { 
      m_code = u;
   }

   const std::string &str() const { 
      return ES09_DEP_STRINGS[ m_code ];
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
