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

//#include <string>
#include "knowledge/tagdict.h"

#define long_1 static_cast<unsigned long long>(1)

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
const string PENN_TAG_STRINGS[] = {
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
   enum {NONE = PENN_TAG_NONE};
   enum {SIZE = PENN_TAG_COUNT_BITS};
   enum {FIRST = PENN_TAG_FIRST};
   enum {LAST = PENN_TAG_COUNT-1};

protected:
   unsigned long m_code;

public:
   CTag() { m_code=PENN_TAG_COUNT; }
   CTag(PENN_TAG_CONSTANTS t) { 
      m_code=t; 
   }
   CTag(int t) { 
      m_code=t; 
   }
   CTag(const string &s) { load(s); }
   virtual ~CTag() {}

public:
   unsigned long code() const { return m_code; }
   string str() const { assert(m_code<PENN_TAG_COUNT) ; return PENN_TAG_STRINGS[m_code]; }
   void load(const string &s) {
      m_code = PENN_TAG_NONE ;
      for (int i=1; i<PENN_TAG_COUNT; ++i)
         if (PENN_TAG_STRINGS[i] == s)
            m_code = i;
   }

public:
   bool operator == (const CTag &t1) const { return m_code == t1.m_code; }
   bool operator < (const CTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CTag &t1) const { return m_code >= t1.m_code; }
};

//=======================================================

/*---------------------------------------------------------------
 *
 * getPossibleTagsByMorphology - get possible tags by morphology 
 *                               information of the word
 *
 *--------------------------------------------------------------*/

inline unsigned long long getPossibleTagsByMorphology( const CTagDict<CTag> &tagdict , const string &word ) {

   const int length = word.size();
   unsigned long long retval = 0;
   unsigned long long stem;

   if ( length < 4 ) return 0;

   // for a word ending with "s"
   if ( word[length-1] == 's' ) {
      
      stem = tagdict.lookup( word.substr( 0, length-1 ) ) ;
      if ( stem & (long_1<<PENN_TAG_NOUN) ) 
         retval |= (long_1<<PENN_TAG_NOUN_PLURAL) ;
      if ( stem & (long_1<<PENN_TAG_VERB) )
         retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE) ;

      // for a word ending with "es"
      if ( word[length-2] == 'e' ) {

         stem = tagdict.lookup( word.substr( 0, length-2 ) ) ;
         if ( stem & (long_1<<PENN_TAG_NOUN) )
            retval |= (long_1<<PENN_TAG_NOUN_PLURAL) ;
         if ( stem & (long_1<<PENN_TAG_VERB) )
            retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE) ;

         // for a word ending with "ies"
         if ( word[length-3] == 'i' ) {
            
            stem = tagdict.lookup( word.substr(0, length-3) + "y" ) ;
            if ( stem & (long_1<<PENN_TAG_NOUN) )
               retval |= (long_1<<PENN_TAG_NOUN_PLURAL) ;
            if ( stem & (long_1<<PENN_TAG_VERB) )
               retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE) ;
            
         }

      }
   }
   else if ( word[length-1] == 'd' && word[length-2] == 'e'){
   
      // a verb with tailing "e" plus "d"
      stem = tagdict.lookup( word.substr( 0, length-1 ) ) ;
      if ( stem & (long_1<<PENN_TAG_VERB) ) {
         retval |= (long_1<<PENN_TAG_VERB_PAST) ; 
         retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ; 
      }
   
      // a verb plus "ed"
      stem = tagdict.lookup( word.substr( 0, length-2 ) ) ;
      if ( stem & (long_1<<PENN_TAG_VERB) ) {
         retval |= (long_1<<PENN_TAG_VERB_PAST) ; 
         retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ; 
      }

      // a verb with the last letter duplicated plus "ed"
      if ( length>4 && word[length-3] == word[length-4] ) {
         stem = tagdict.lookup( word.substr( 0, length-3 ) ) ;
         if ( stem & (long_1<<PENN_TAG_VERB) ) {
            retval |= (long_1<<PENN_TAG_VERB_PAST) ; 
            retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ;
         }
      }

   }
   else if ( word[length-1] == 'g' && word[length-2] == 'n' && word[length-3] == 'i' ) {

      // a verb plus ing
      stem = tagdict.lookup( word.substr( 0, length-3 ) ) ;
      if ( stem & (long_1<<PENN_TAG_VERB) ) 
         retval |= (long_1<<PENN_TAG_VERB_PROG) ; 

      // a verb with the last letter repeated when it does ing
      if ( length>5 && word[length-4] == word[length-5] ) {
         stem = tagdict.lookup( word.substr(0, length-4) ) ;
         if ( stem & (long_1<<PENN_TAG_VERB) )
            retval |= (long_1<<PENN_TAG_VERB_PROG) ;
      }

   }
   else if ( word[length-1] == 'r' && word[length-2] == 'e' ) {
      
      // an adjective with tailing "e" plus "r"
      stem = tagdict.lookup( word.substr( 0, length-1 ) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) ) 
         retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ; 
   
      // an adjective plus "er"
      stem = tagdict.lookup( word.substr( 0, length-2 ) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) ) 
         retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ; 

      // an adjective with last letter as "y" and switch to "ier"
      stem = tagdict.lookup( word.substr(0, length-3) + "y" ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
         retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ;

   }
   else if ( word[length-1] == 't' && word[length-2] == 's' && word[length-3] == 'e' ) {

      // an adjective with tailing "e" plus "st"
      stem = tagdict.lookup( word.substr( 0, length-2 ) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) ) 
         retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ; 
   
      // an adjective plus "est"
      stem = tagdict.lookup( word.substr( 0, length-3 ) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) ) 
         retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ; 

      // an adjective with last letter as "y" and switch to "iest"
      if ( length>4 ) {
         stem = tagdict.lookup( word.substr(0, length-4) + "y" ) ;
         if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
            retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ;
      }

   }

   return retval;

}

/*---------------------------------------------------------------
 *
 * getPossibleTagsBySuffix - get possible tags by morphology 
 *                           information of the word
 *
 *--------------------------------------------------------------*/

inline unsigned long long getPossibleTagsBySuffix( const string &word ) {

   unsigned long long retval=0;
   const int length=word.size();

   if ( length<4 ) return 0;

   if ( word[length-1] == 's' ) {
      retval |= (long_1<<PENN_TAG_NOUN_PLURAL);
      retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE);
   }
   else if ( word[length-2] == 'e' && word[length-1] == 'd'){
      retval |= (long_1<<PENN_TAG_VERB_PAST) ; 
      retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ; 
   }
   else if ( word[length-3] == 'i' && word[length-2] == 'n' && word[length-1] == 'g' ) {
      retval |= (long_1<<PENN_TAG_VERB_PROG) ; 
   }
   else if ( word[length-2] == 'e' && word[length-1] == 'r' ) {
      retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ; 
      retval |= (long_1<<PENN_TAG_ADVERB_COMPARATIVE) ; 
   }
   else if ( word[length-3] == 'e' && word[length-2] == 's' && word[length-1] == 't' ) {
      retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ; 
      retval |= (long_1<<PENN_TAG_ADVERB_SUPERLATIVE) ; 
   }

   return retval;
}

}; // namespace english

#endif

