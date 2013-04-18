// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * sentence_string.h - the definitions for raw sentences        *
 *                                                              *
 * use the modules reader and writer for different types of io. *
 * the chinese sentences from this file are unicode             *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.3                         *
 *                                                              *
 ****************************************************************/

#ifndef _SENTENCE_STRING_H
#define _SENTENCE_STRING_H

#include "utf.h"

/*===============================================================
 *
 * Type definitions
 *
 *==============================================================*/

typedef std::vector< std::string > CStringVector ;

typedef std::vector< std::pair< std::string , std::string > > CTwoStringVector ;

/*===============================================================
 *
 * Sentnece utilities
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * wordsEqual - decide if two tagged sentences has the same words
 *              
 *---------------------------------------------------------------*/

inline
bool wordsEqual(CTwoStringVector *snt1, CTwoStringVector *snt2) {
   if (snt1->size() != snt2->size()) return false;
   for (unsigned long int i=0; i<snt1->size(); ++i)
      if (snt1->at(i).first != snt2->at(i).first) return false;
   return true;
}

/*---------------------------------------------------------------
 *
 * DesegmentSentence - join sentence
 *
 * This methods makes a new instance of raw sentence.
 *
 *--------------------------------------------------------------*/

inline 
void DesegmentSentence(const CStringVector *sent, CStringVector *retval) {
   assert(retval != 0);
   retval->clear();
   if (sent == 0)
      return;
   CStringVector::const_iterator it;
   for (it=sent->begin(); it!=sent->end(); ++it)
      getCharactersFromUTF8String(*it, retval);
}

/*---------------------------------------------------------------
 *
 * UntagSentence - remove tags
 *
 * Input: sTagged - tagged sentence as input to methods
 *
 * Return: segmented form
 *
 *--------------------------------------------------------------*/

inline
void UntagSentence(const CTwoStringVector *tagged, CStringVector *retval) {
   assert(retval != 0);
   retval->clear();
   if (tagged == 0)
      return;
   CTwoStringVector::const_iterator it;
   for (it=tagged->begin(); it!=tagged->end(); ++it)
      retval->push_back(it->first);
}


/*---------------------------------------------------------------
 *
 * UntagSentence - remove tags
 *
 * Input: sTagged - tagged sentence as input to methods
 *
 * Return: segmented form, tags
 *
 *--------------------------------------------------------------*/

inline
void UntagSentenceSaveTag(const CTwoStringVector *tagged, CStringVector *retval, CStringVector *retvalpos) {
   assert(retval != 0);
   retval->clear();
   assert(retvalpos != 0);
   retvalpos->clear();
   if (tagged == 0)
      return;
   CTwoStringVector::const_iterator it;
   for (it=tagged->begin(); it!=tagged->end(); ++it)
   {
      retval->push_back(it->first);
      retvalpos->push_back(it->second);
   }
}

/*---------------------------------------------------------------
 *
 * UntagAndDesegmentSentence - remove tags and join
 *
 * Input: sTagged - tagged sentence as input to methods
 *
 * Return: raw form
 *
 *--------------------------------------------------------------*/

inline
void UntagAndDesegmentSentence(const CTwoStringVector *tagged, CStringVector *retval) {
   assert(retval != 0);
   retval->clear();
   if (tagged == 0)
      return;
   std::string temp;
   CTwoStringVector::const_iterator it;
   for (it = tagged->begin(); it != tagged->end(); ++it) {
      getCharactersFromUTF8String(it->first, retval);
   }
}

/*---------------------------------------------------------------
 *
 * DesegmentSentence - join sentence
 *
 * This methods makes a new instance of raw sentence.
 *
 *--------------------------------------------------------------*/

template<class CCharCatDict>
inline 
void DesegmentSentence(const CStringVector *sent, CStringVector *retval, CCharCatDict &char_categories) {
   assert(retval != 0);
   retval->clear();
   if (sent == 0)
      return;
   std::string temp;
   CStringVector::const_iterator it;
   for (it=sent->begin(); it!=sent->end(); ++it) {
      if ( retval->size() > 0 ) {
         temp = getFirstCharFromUTF8String( *it ) ; 
         std::string &last = retval->at( retval->size()-1 ) ; 
         if ( char_categories.isFWorCD( last ) &&
              char_categories.isFWorCD( temp ) )
            retval->push_back(" ");
      }
      getCharactersFromUTF8String(*it, retval); 
   }
}

/*---------------------------------------------------------------
 *
 * UntagAndDesegmentSentence - remove tags and join
 *
 * Input: sTagged - tagged sentence as input to methods
 *
 * Return: raw form
 *
 *--------------------------------------------------------------*/

template<class CCharCatDict>
inline
void UntagAndDesegmentSentence(const CTwoStringVector *tagged, CStringVector *retval, const CCharCatDict &dict) {
   assert(retval != 0);
   retval->clear();
   if (tagged == 0)
      return;
   std::string temp;
   CTwoStringVector::const_iterator it;
   for (it = tagged->begin(); it != tagged->end(); ++it) {
      // special process!! if the two words are numbers or foreign words, 
      // we need to separate them manually by adding a space
      if (retval->size()>0) {
         temp = getFirstCharFromUTF8String(it->first);
         std::string &last = retval->at( retval->size()-1 );
         if ( ( dict.isFWorCD( last ) )  &&
              ( dict.isFWorCD( temp ) )
            ) {
            retval->push_back(" ");
         }
      }
      // the normal process with adding the word to sentence
      getCharactersFromUTF8String(it->first, retval);
   }
}

#endif

