// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tags.h - the definitions for english tags                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _ENGLISH_PENN_POS_H
#define _ENGLISH_PENN_POS_H 1

namespace english {

/*===============================================================
 *
 * definitions about tags
 *
 * CTag is defined as unsigned long integer, which is easier to store.
 * The value of CTag is defined by the index in PENN_TAG_STRINGS + 1.
 *
 *==============================================================*/

// the penn tag set
// Modify the following three constants together, keeping consistency!
const std::string PENN_TAG_STRINGS[] = {
   "-NONE-",
   "-BEGIN-",
   "-END-",
   "$", "``", "''",
   "-LRB-", "-RRB-", ",", // note that brackets can sometimes be ( and )
   ".", ":", "#",
   "CC", "CD", "DT", "EX", "FW",
   "IN", "JJ", "JJR", "JJS", "LS",
   "MD", "NN", "NNP", "NNPS", "NNS",
   "PDT", "POS", "PRP", "PRP$",
   "RB", "RBR", "RBS", "RP",
   "SYM", "TO", "UH",
   "VB", "VBD", "VBG", "VBN", "VBP", "VBZ",
   "WDT", "WP", "WP$", "WRB"
};

enum PENN_TAG_CONSTANTS {
   PENN_TAG_NONE=0,
   PENN_TAG_BEGIN,
   PENN_TAG_END,
   PENN_TAG_DOLLAR, PENN_TAG_L_QUOTE, PENN_TAG_R_QUOTE,
   PENN_TAG_L_BRACKET, PENN_TAG_R_BRACKET, PENN_TAG_COMMA,
   PENN_TAG_PERIOD, PENN_TAG_COLUM, PENN_TAG_SHART,
   PENN_TAG_CC, PENN_TAG_CD, PENN_TAG_DT, PENN_TAG_EX, PENN_TAG_FW,
   PENN_TAG_IN, PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE, PENN_TAG_LS,
   PENN_TAG_MD, PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
   PENN_TAG_PDT, PENN_TAG_POS, PENN_TAG_PRP, PENN_TAG_PRP_DOLLAR,
   PENN_TAG_ADVERB, PENN_TAG_ADVERB_COMPARATIVE, PENN_TAG_ADVERB_SUPERLATIVE, PENN_TAG_RP,
   PENN_TAG_SYM, PENN_TAG_TO, PENN_TAG_UH,
   PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
   PENN_TAG_WDT, PENN_TAG_WP, PENN_TAG_WP_DOLLAR, PENN_TAG_WRB,
   PENN_TAG_COUNT
};

const bool PENN_TAG_CLOSED[] = {
   false,
   true,
   true,
   true, true, true,
   true, true, true,
   true, true, true,
   true, false, true, true, false,
   true, false, false, false, false,
   true, false, false, false, false,
   true, true, true, true,
   false, false, false, false,
   false, true, false,
   false, false, false, false, false, false,
   true, true, true, true
};

const int PENN_TAG_FIRST = 3;
const int PENN_TAG_COUNT_BITS = 6;

const unsigned long long PENN_TAG_MUST_SEE = (static_cast<unsigned long long>(1)<<PENN_TAG_SYM) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_FW) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_CD) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_LS) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_NOUN_PROPER) | \
                                        (static_cast<unsigned long long>(1)<<PENN_TAG_NOUN_PROPER_PLURAL) ;

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
   CTag(int t) : m_code(t) { }
   CTag(const std::string &s) { load(s); }
   virtual ~CTag() {}

public:
   unsigned long code() const { return m_code; }
   unsigned long hash() const { return m_code; }
   void copy(const CTag &t) { m_code = t.m_code; }
   std::string str() const { assert(m_code<PENN_TAG_COUNT) ; return PENN_TAG_STRINGS[m_code]; }
   void load(const std::string &s) {
      m_code = PENN_TAG_NONE ;
      for (int i=1; i<PENN_TAG_COUNT; ++i)
         if (PENN_TAG_STRINGS[i] == s)
            m_code = i;
   }
   void load(const unsigned &i) {
      m_code = i;
   }
   bool closed() const { return PENN_TAG_CLOSED[m_code]; }

public:
   bool operator == (const CTag &t1) const { return m_code == t1.m_code; }
   bool operator != (const CTag &t1) const { return m_code != t1.m_code; }
   bool operator < (const CTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CTag &t1) const { return m_code >= t1.m_code; }
};

//===============================================================

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

}; // namespace english

#endif

