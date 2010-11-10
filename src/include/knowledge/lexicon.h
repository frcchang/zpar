// Copyright (C) University of Oxford 2010
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

class CLexiconSet : public CHashMap<CWord, unsigned long> {
public:
   CLexiconSet(const unsigned &size=65537) : CHashMap<CWord, unsigned long>(size) {}
public:
   void add(const CWord &word) { (*this)[word]=1; }
   bool contains(const CWord &word) { return static_cast<bool>(find(word, 0)); }
};

inline
std::istream & operator >> (std::istream &is, CLexiconSet &dict) {
   if (!is) return is;
   std::string s ;
   std::string ln;
   getline(is, ln);
   while (is && !(ln.empty())) {
      std::istringstream iss(ln);
      iss >> s; 
      dict.add(CWord(s));
      getline(is, ln);
   }
   return is;
}

inline
std::ostream & operator << (std::ostream &os, CLexiconSet &dict) {
   CLexiconSet::iterator it = dict.begin();
   while (it != dict.end()) {
      os << it.first().str() << std::endl;
      ++it;
   }
   os << std::endl;
   return os;
}

#endif
