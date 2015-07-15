// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * morphology.h - the knowledge about word morph.               *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _ENGLISH_MORPHOLOGY_H
#define _ENGLISH_MORPHOLOGY_H

#include "knowledge/tagdict.h"

#define long_1 static_cast<unsigned long long>(1)

namespace english {

/*---------------------------------------------------------------
 *
 * getPossibleTagsByMorphology - get possible tags by morphology
 *                               information of the word
 *
 *--------------------------------------------------------------*/
/*
unsigned long long getPossibleTagsByMorphology( const CTagDict<CWord, CTag> &tagdict , const std::string &word ) {

   const int length = word.size();
   unsigned long long retval = 0;
   unsigned long long stem;

   if ( length < 4 ) return 0;

   // for a word ending with "s"
   if ( word[length-1] == 's' ) {

      stem = tagdict.lookup( CWord(word.substr( 0, length-1 )) ) ;
      if ( stem & (long_1<<PENN_TAG_NOUN) )
         retval |= (long_1<<PENN_TAG_NOUN_PLURAL) ;
      if ( stem & (long_1<<PENN_TAG_VERB) )
         retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE) ;

      // for a word ending with "es"
      if ( word[length-2] == 'e' ) {

         stem = tagdict.lookup( CWord(word.substr( 0, length-2 )) ) ;
         if ( stem & (long_1<<PENN_TAG_NOUN) )
            retval |= (long_1<<PENN_TAG_NOUN_PLURAL) ;
         if ( stem & (long_1<<PENN_TAG_VERB) )
            retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE) ;

         // for a word ending with "ies"
         if ( word[length-3] == 'i' ) {

            stem = tagdict.lookup( CWord(word.substr(0, length-3) + "y") ) ;
            if ( stem & (long_1<<PENN_TAG_NOUN) )
               retval |= (long_1<<PENN_TAG_NOUN_PLURAL) ;
            if ( stem & (long_1<<PENN_TAG_VERB) )
               retval |= (long_1<<PENN_TAG_VERB_THIRD_SINGLE) ;

         }

      }
   }
   else if ( word[length-1] == 'd' && word[length-2] == 'e'){

      // a verb with tailing "e" plus "d"
      stem = tagdict.lookup( CWord(word.substr( 0, length-1 )) ) ;
      if ( stem & (long_1<<PENN_TAG_VERB) ) {
         retval |= (long_1<<PENN_TAG_VERB_PAST) ;
         retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ;
      }

      // a verb plus "ed"
      stem = tagdict.lookup( CWord(word.substr( 0, length-2 )) ) ;
      if ( stem & (long_1<<PENN_TAG_VERB) ) {
         retval |= (long_1<<PENN_TAG_VERB_PAST) ;
         retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ;
      }

      // a verb with the last letter duplicated plus "ed"
      if ( length>4 && word[length-3] == word[length-4] ) {
         stem = tagdict.lookup( CWord(word.substr( 0, length-3 )) ) ;
         if ( stem & (long_1<<PENN_TAG_VERB) ) {
            retval |= (long_1<<PENN_TAG_VERB_PAST) ;
            retval |= (long_1<<PENN_TAG_VERB_PAST_PARTICIPATE) ;
         }
      }

   }
   else if ( word[length-1] == 'g' && word[length-2] == 'n' && word[length-3] == 'i' ) {

      // a verb plus ing
      stem = tagdict.lookup( CWord(word.substr( 0, length-3 )) ) ;
      if ( stem & (long_1<<PENN_TAG_VERB) )
         retval |= (long_1<<PENN_TAG_VERB_PROG) ;

      // a verb with the last letter repeated when it does ing
      if ( length>5 && word[length-4] == word[length-5] ) {
         stem = tagdict.lookup( CWord(word.substr(0, length-4)) ) ;
         if ( stem & (long_1<<PENN_TAG_VERB) )
            retval |= (long_1<<PENN_TAG_VERB_PROG) ;
      }

   }
   else if ( word[length-1] == 'r' && word[length-2] == 'e' ) {

      // an adjective with tailing "e" plus "r"
      stem = tagdict.lookup( CWord(word.substr( 0, length-1 )) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
         retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ;

      // an adjective plus "er"
      stem = tagdict.lookup( CWord(word.substr( 0, length-2 )) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
         retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ;

      // an adjective with last letter as "y" and switch to "ier"
      stem = tagdict.lookup( CWord(word.substr(0, length-3) + "y") ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
         retval |= (long_1<<PENN_TAG_ADJECTIVE_COMPARATIVE) ;

   }
   else if ( word[length-1] == 't' && word[length-2] == 's' && word[length-3] == 'e' ) {

      // an adjective with tailing "e" plus "st"
      stem = tagdict.lookup( CWord(word.substr( 0, length-2 )) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
         retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ;

      // an adjective plus "est"
      stem = tagdict.lookup( CWord(word.substr( 0, length-3 )) ) ;
      if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
         retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ;

      // an adjective with last letter as "y" and switch to "iest"
      if ( length>4 ) {
         stem = tagdict.lookup( CWord(word.substr(0, length-4) + "y") ) ;
         if ( stem & (long_1<<PENN_TAG_ADJECTIVE) )
            retval |= (long_1<<PENN_TAG_ADJECTIVE_SUPERLATIVE) ;
      }

   }

   return retval;

}
*/

/*---------------------------------------------------------------
 *
 * getPossibleTagsBySuffix - get possible tags by morphology
 *                           information of the word
 *
 *--------------------------------------------------------------*/

inline unsigned long long getPossibleTagsBySuffix( const std::string &word ) {

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

};

#endif
