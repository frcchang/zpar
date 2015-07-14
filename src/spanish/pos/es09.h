/**
 * Spanish POS tags from the AnCoRa 2.0 (and CoNLL 2009) tagset
 *
 * Carlos Gomez-Rodriguez, Yue Zhang
 */


#ifndef _ES09_POS_H
#define _ES09_POS_H 1

#include <assert.h>

namespace spanish {

/*===============================================================
 *
 * definitions about tags
 *
 * CTag is defined as unsigned long integer, which is easier to store.
 * The value of CTag is defined by the index in ES09_TAG_STRINGS + 1.
 *
 *==============================================================*/

// the tag set
// Modify the following three constants together, keeping consistency!
const std::string ES09_TAG_STRINGS[] = {
		"-NONE-",
		"-BEGIN-",
		"-END-",

		"a",
		"c",
		"d",
		"f",
		"i",
		"n",
		"p",
		"r",
		"s",
		"v",
		"w",
		"z"
};

enum ES09_TAG_CONSTANTS {

	ES09_TAG_NONE=0,
	ES09_TAG_BEGIN,
	ES09_TAG_END,

	ES09_TAG_ADJ,
	ES09_TAG_CONJ,
	ES09_TAG_DET,
	ES09_TAG_PUNCT,
	ES09_TAG_INTERJ,
	ES09_TAG_NOUN,
	ES09_TAG_PRON,
	ES09_TAG_ADV,
	ES09_TAG_PREP,
	ES09_TAG_VERB,
	ES09_TAG_DATE,
	ES09_TAG_NUMBER,

	ES09_TAG_COUNT
};

const bool ES09_TAG_CLOSED[] = {
   false,
   true,
   true,

   false,
   true,
   true,
   true,
   false,
   false,
   true,
   false,
   true,
   false,
   false,
   false

   //count doesn't have a constant here
};

const int ES09_TAG_FIRST = 3;
const int ES09_TAG_COUNT_BITS = 4; //phew, that's close

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
   enum {SENTENCE_BEGIN = ES09_TAG_BEGIN};
   enum {SENTENCE_END = ES09_TAG_END};
   enum {COUNT = ES09_TAG_COUNT};
   enum {MAX_COUNT = ES09_TAG_COUNT};
   enum {NONE = ES09_TAG_NONE};
   enum {SIZE = ES09_TAG_COUNT_BITS};
   enum {FIRST = ES09_TAG_FIRST};
   enum {LAST = ES09_TAG_COUNT-1};

protected:
   unsigned long m_code;

public:
   CTag() : m_code(NONE) {}
   CTag(ES09_TAG_CONSTANTS t) : m_code(t) { }
   CTag(int t) : m_code(t) { }
   CTag(const std::string &s) { load(s); }
   virtual ~CTag() {}

public:
   unsigned long code() const { return m_code; }
   unsigned long hash() const { return m_code; }
   void copy(const CTag &t) { m_code = t.m_code; }
   std::string str() const { assert(m_code<ES09_TAG_COUNT) ; return ES09_TAG_STRINGS[m_code]; }
   void load(const std::string &s) {
      m_code = ES09_TAG_NONE ;
      for (int i=1; i<ES09_TAG_COUNT; ++i)
         if (ES09_TAG_STRINGS[i] == s)
            m_code = i;
   }
   void load(const unsigned &i) {
      m_code = i;
   }
   bool closed() const { return ES09_TAG_CLOSED[m_code]; }

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

}; // namespace spanish

#endif

