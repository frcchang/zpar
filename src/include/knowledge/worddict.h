/****************************************************************
 *                                                              *
 * worddict.h - the dictionary from word to unsigned int.       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _WORD_DICTIONARY_H
#define _WORD_DICTIONARY_H

#include "hash.h"

static const unsigned int DICT_SIZE = 16381 ;

typedef unsigned int CWordCat;

/*===============================================================
 *
 * CWordDictionary - the dictionary
 *
 *==============================================================*/

class CWordDictionary : public CHashMap<CWord, CWordCat> {

public:

   CWordDictionary(unsigned size = DICT_SIZE) : CHashMap<CWord, CWordCat>(size) { }
   virtual ~CWordDictionary() { }

public:

   // whether the word and tag pair is in the dictionary
   CWordCat lookup(const CWord &word) const {
      return find(word, 0);
   }

public:

   // load tag dictionary from a file
   void load(const string &sFileName) {
      ifstream ifs ;
      string s ;
      unsigned int i ;

      ifs.open( sFileName.c_str() ) ;
      assert( ifs.is_open() ) ;
      while( !ifs.eof() ) {
         ifs >> s >> i ;
         (*this)[s] = i ;
      }
      ifs.close() ;
   }

   // save tag dictionary to a file
   void save(const string &sFileName) {

      ofstream ofs ;
      iterator it;

      ofs.open( sFileName.c_str() ) ;
      assert( ofs.is_open() ) ;
      it = begin() ;
      while (it != end()) {
         ofs << it.first().str() << "\t" << it.second() << endl ;
         ++it ; 
      }
      ofs.close() ;

   }
};

#endif
