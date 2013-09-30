/*
 * morph_pos.h
 *
 *  Created on: Sep 18, 2013
 *      Author: carlos
 */

#ifndef MORPH_POS_H_
#define MORPH_POS_H_


namespace english
{

// the simplified tagset for the morphological analyzer
const std::string MORPH_TAG_STRINGS[] = {
   "-NONE-",
   "-BEGIN-",
   "-END-",

   "$", "``", "''",
   "-LRB-", "-RRB-", ",", // note that brackets can sometimes be ( and )
   ".", ":", "#",

//   "Punct",

   "Noun",
   "Verb", "Modal Verb",
   "Adj",
   "PRP", "PRP$", "WP", "WP$", //at the moment we don't split on possessive, wh
   "Det", "WH Det", //at the moment we don't split on wh
   "Adv", "WH Adv",
   "POS", "IN", "Coord Conj", "TO",
   "Num",
   "Interj",

   "Ex",
   "Foreign",
   "ListItem",
   "Predet",
   "Particle",
   "Symbol"
};

enum MORPH_TAG_CONSTANTS {
	MORPH_TAG_NONE=0,
	MORPH_TAG_BEGIN,
	MORPH_TAG_END,

//	MORPH_TAG_PUNCT,

	MORPH_TAG_DOLLAR, MORPH_TAG_OPEN_QUOTE, MORPH_TAG_CLOSE_QUOTE,
	MORPH_TAG_LRB, MORPH_TAG_RRB, MORPH_TAG_COMMA,
	MORPH_TAG_PERIOD, MORPH_TAG_COLON, MORPH_TAG_HASH,

	MORPH_TAG_NOUN,
	MORPH_TAG_VERB, MORPH_TAG_MODAL_VERB,
	MORPH_TAG_ADJ,
	MORPH_TAG_PRON, MORPH_TAG_POSS_PRON, MORPH_TAG_WH_PRON, MORPH_TAG_WH_POSS_PRON,
	MORPH_TAG_DET, MORPH_TAG_WH_DET,
	MORPH_TAG_ADV, MORPH_TAG_WH_ADV,
	MORPH_TAG_POS, MORPH_TAG_IN, MORPH_TAG_CC, MORPH_TAG_TO,
	MORPH_TAG_NUM,
	MORPH_TAG_INTERJ,

	MORPH_TAG_EX,
	MORPH_TAG_FOREIGN,
	MORPH_TAG_LIST,
	MORPH_TAG_PREDET,
	MORPH_TAG_PARTICLE,
	MORPH_TAG_SYM,

	MORPH_TAG_COUNT
};

const bool MORPH_TAG_CLOSED[] = {
   false,
   true,
   true,

//false,

   true, true, true,
   true, true, true,
   true, true, true,

   false,
   false, false,
   false,
   true, true, true, true,
   true, true,
   false, false,
   true, true, true, true,
   false,
   false,

   true,
   false,
   false,
   false,
   true,
   false
};

const int MORPH_TAG_FIRST = 3;
const int MORPH_TAG_COUNT_BITS = 6;

/**
 * Convert a Penn Treebank POS tag into one of our morphological analyzer's POS tags.
 */
int pennToMorphTag(const std::string &pennTag);

/**
 * Convert a Multext detailed POS tag into one of our morphological analyzer's POS tags.
 * Unused at the moment.
 */
int multextToMorphtag(const std::string &multextTag); //several tags? e.g. there: EX, but also adverb



class CMorphTag {
public:
   enum {SENTENCE_BEGIN = MORPH_TAG_BEGIN};
   enum {SENTENCE_END = MORPH_TAG_END};
   enum {COUNT = MORPH_TAG_COUNT};
   enum {MAX_COUNT = MORPH_TAG_COUNT};
   enum {NONE = MORPH_TAG_NONE};
   enum {SIZE = MORPH_TAG_COUNT_BITS};
   enum {FIRST = MORPH_TAG_FIRST};
   enum {LAST = MORPH_TAG_COUNT-1};

protected:
   unsigned long m_code;

public:
   CMorphTag() : m_code(NONE) {}
   CMorphTag(MORPH_TAG_CONSTANTS t) : m_code(t) { }
   CMorphTag(int t) : m_code(t) { }
   CMorphTag(const std::string &s) { load(s); }
   virtual ~CMorphTag() {}

public:
   unsigned long code() const { return m_code; }
   unsigned long hash() const { return m_code; }
   void copy(const CMorphTag &t) { m_code = t.m_code; }
   std::string str() const { assert(m_code<MORPH_TAG_COUNT) ; return MORPH_TAG_STRINGS[m_code]; }
   void load(const std::string &s) {
      m_code = MORPH_TAG_NONE ;
      for (int i=1; i<MORPH_TAG_COUNT; ++i)
         if (MORPH_TAG_STRINGS[i] == s)
            m_code = i;
   }
   void load(const unsigned &i) {
      m_code = i;
   }
   bool closed() const { return MORPH_TAG_CLOSED[m_code]; }

public:
   bool operator == (const CMorphTag &t1) const { return m_code == t1.m_code; }
   bool operator != (const CMorphTag &t1) const { return m_code != t1.m_code; }
   bool operator < (const CMorphTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CMorphTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CMorphTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CMorphTag &t1) const { return m_code >= t1.m_code; }
};





}



#endif /* MORPH_POS_H_ */
