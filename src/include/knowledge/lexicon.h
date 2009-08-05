/****************************************************************
 *                                                              *
 * lexicon.h - the lexicon dictionary equals to a set of words. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.03                        *
 *                                                              *
 ****************************************************************/

#ifndef _DICT_LEXICON_H
#define _DICT_LEXICON_H

#include "hash.h"
#include "linguistics/word.h"

class CLexiconSet : public CHashMap<CWord, unsigned> {
public:
   void add(const CWord &word) { (*this)[word]=1; }
   bool contains(const CWord &word) { return static_cast<bool>(find(word, 0)); }
};

inline
istream & operator >> (istream &i, CLexiconSet &dict) {
   string s ;
   while (i) {
      i >> s; assert(s!="");
      dict.add(CWord(s));
   }
   return i;
}

#endif
