w/**
 * Spanish POS tags from the CoNLL 2006 (CoNLL-X) tagset
 *
 * Carlos Gomez-Rodriguez, Yue Zhang
 */


#ifndef _ES06_POS_H
#define _ES06_POS_H 1

#include <assert.h>

namespace spanish {

/*===============================================================
 *
 * definitions about tags
 *
 * CTag is defined as unsigned long integer, which is easier to store.
 * The value of CTag is defined by the index in ES06_TAG_STRINGS + 1.
 *
 *==============================================================*/

// the tag set
// Modify the following three constants together, keeping consistency!
const std::string ES06_TAG_STRINGS[] = {
		"-NONE-",
		"-BEGIN-",
		"-END-",

		"Fa", "Fc", "Fd", "Fe", "Fg", "Fh", "Fi", "Fp", "Fs", "Fx", "Fz", //punctuation
		"X",
		"Y",
		"Zm", "Zp",
		"ao", "aq",
		"cc", "cs",
		"da", "dd", "de", "di", "dn", "dp", "dt",
		"i",
		"nc", "np",
		"p0", "pd", "pe", "pi", "pn", "pp", "pr", "pt", "px",
		"rg", "rn",
		"sn",
		"sp",
		"va", "vm", "vs",
		"w",
		"z"
};

enum ES06_TAG_CONSTANTS {

	ES06_TAG_NONE=0,
	ES06_TAG_BEGIN,
	ES06_TAG_END,

	ES06_TAG_FA, ES06_TAG_FC, ES06_TAG_FD, ES06_TAG_FE, ES06_TAG_FG, ES06_TAG_FH, ES06_TAG_FI, ES06_TAG_FP, ES06_TAG_FS, ES06_TAG_FX, ES06_TAG_FZ,
	ES06_TAG_FOREIGN,
	ES06_TAG_Y,
	ES06_TAG_ZM, ES06_TAG_ZP,
	ES06_TAG_ADJ_ORD, ES06_TAG_ADJ_QUAL,
	ES06_TAG_CONJ_COORD, ES06_TAG_CONJ_SUB,
	ES06_TAG_DET_ART, ES06_TAG_DET_DEM, ES06_TAG_DET_EXC, ES06_TAG_DET_IND, ES06_TAG_DET_NUM, ES06_TAG_DET_POS, ES06_TAG_DET_INTERROG,
	ES06_TAG_INTERJ,
	ES06_TAG_NOUN_COMMON, ES06_TAG_NOUN_PROPER,
	ES06_TAG_PRON_0, ES06_TAG_PRON_DEM, ES06_TAG_PRON_EXCL, ES06_TAG_PRON_IND, ES06_TAG_PRON_NUM, ES06_TAG_PRON_PERS, ES06_TAG_PRON_REL, ES06_TAG_PRON_INTERROG, ES06_TAG_PRON_POS,
	ES06_TAG_ADV_GEN, ES06_TAG_ADV_NEG,
	ES06_TAG_ELLIPTIC_SUBJECT,
	ES06_TAG_PREP,
	ES06_TAG_VERB_HABER, ES06_TAG_VERB, ES06_TAG_VERB_SER,
	ES06_TAG_DATE,
	ES06_TAG_NUMBER,

	ES06_TAG_COUNT
};

const bool ES06_TAG_CLOSED[] = {
   false,
   true,
   true,
   true, true, true, true, true, true, true, true, true, true, true,
   false,
   false,
   false, false,
   false, false,
   true, true,
   true, true, true, true, true, true, true,
   false,
   false, false,
   true, true, true, true, true, true, true, true, true,
   false, false,
   true,
   true,
   true, false, true,
   false,
   false

   //count doesn't have a constant here
};

const int ES06_TAG_FIRST = 3;
const int ES06_TAG_COUNT_BITS = 6;

//TODO: what is this?
/*
const unsigned long long PENN_TAG_MUST_SEE = (static_cast<unsigned long long>(1)<<PENN_TAG_SYM) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_FW) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_CD) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_LS) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_NOUN_PROPER) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_NOUN_PROPER_PLURAL) ;
*/

//===============================================================

class CTag {
public:
   enum {SENTENCE_BEGIN = ES06_TAG_BEGIN};
   enum {SENTENCE_END = ES06_TAG_END};
   enum {COUNT = ES06_TAG_COUNT};
   enum {MAX_COUNT = ES06_TAG_COUNT};
   enum {NONE = ES06_TAG_NONE};
   enum {SIZE = ES06_TAG_COUNT_BITS};
   enum {FIRST = ES06_TAG_FIRST};
   enum {LAST = ES06_TAG_COUNT-1};

protected:
   unsigned long m_code;

public:
   CTag() : m_code(NONE) {}
   CTag(ES06_TAG_CONSTANTS t) : m_code(t) { }
   CTag(int t) : m_code(t) { }
   CTag(const std::string &s) { load(s); }
   virtual ~CTag() {}

public:
   unsigned long code() const { return m_code; }
   unsigned long hash() const { return m_code; }
   void copy(const CTag &t) { m_code = t.m_code; }
   std::string str() const { assert(m_code<ES06_TAG_COUNT) ; return ES06_TAG_STRINGS[m_code]; }
   void load(const std::string &s) {
      m_code = ES06_TAG_NONE ;
      for (int i=1; i<ES06_TAG_COUNT; ++i)
         if (ES06_TAG_STRINGS[i] == s)
            m_code = i;
   }
   void load(const unsigned &i) {
      m_code = i;
   }
   bool closed() const { return ES06_TAG_CLOSED[m_code]; }

public:
   bool operator == (const CTag &t1) const { return m_code == t1.m_code; }
   bool operator != (const CTag &t1) const { return m_code != t1.m_code; }
   bool operator < (const CTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CTag &t1) const { return m_code >= t1.m_code; }
};

//===============================================================

//TODO It seems that this is only used by "punct" implementation, so we do not include it.
/*
inline unsigned matchPunctuation(const CTag &tag) {
   if (tag==PENN_TAG_L_QUOTE) return 1;
   else if (tag == PENN_TAG_L_BRACKET) return 2;
   else if (tag==PENN_TAG_R_BRACKET) return 4;
   else if (tag == PENN_TAG_COMMA) return 8;
   else if (tag == PENN_TAG_PERIOD) return 16;
   else if (tag == PENN_TAG_COLUM) return 32;
   else if (tag == PENN_TAG_R_QUOTE) return 64;
   else return 0;
}
*/

//Punctuation tags:
//ES06_TAG_FA, ES06_TAG_FC, ES06_TAG_FD, ES06_TAG_FE, ES06_TAG_FG, ES06_TAG_FH, ES06_TAG_FI, ES06_TAG_FP, ES06_TAG_FS, ES06_TAG_FX, ES06_TAG_FZ,

}; // namespace spanish

#endif

