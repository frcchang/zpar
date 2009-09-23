/****************************************************************
 *                                                              *
 * worddict.h - the dictionary from word to unsigned long int.       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _WORD_DICTIONARY_H
#define _WORD_DICTIONARY_H

#include "hash.h"

static const unsigned long DICT_SIZE = 16381 ;

typedef unsigned long CWordCat;

/*===============================================================
 *
 * CWordDictionary - the dictionary
 *
 *==============================================================*/

class CWordDictionary : public CHashMap<CWord, CWordCat> {

public:

   CWordDictionary(unsigned long size = DICT_SIZE) : CHashMap<CWord, CWordCat>(size) { }
   virtual ~CWordDictionary() { }

public:

   // whether the word and tag pair is in the dictionary
   CWordCat lookup(const CWord &word) const {
      return find(word, 0);
   }

};

//==============================================================*/

inline
istream & operator >> (istream &is, CWordDictionary &dict) {
   if (!is) return is;
   string s ;
   unsigned long int i ;
   string ln ; 
   getline(is, ln);
   while (is && !(ln.empty())) {
      istringstream iss(ln) ; 
      iss >> s >> i ;
      dict[s] = i ;
      getline(is, ln);
   }
   return is ;
}

inline
ostream & operator << (ostream &os, CWordDictionary &dic) {
   CWordDictionary::iterator it;
   it = dic.begin() ;
   while (it != dic.end()) {
      os << it.first().str() << "\t" << it.second() << endl ;
      ++it ; 
   }
   os << endl;
   return os ;
}

#endif
