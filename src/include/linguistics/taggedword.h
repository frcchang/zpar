// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * taggedword.h - the definitions for tagged general words      *
 *                                                              *
 * A tagged word is a word with a tag. Since the definition of  *
 * CWord can be implemented in different ways, a proper file    *
 * that defines CWord must be included before using this file.  *
 * This file presumes that CWord has already been defined.      *
 * Similarly, CTag must have been defined before including this *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _TAGGED_WORD_H
#define _TAGGED_WORD_H

/*===============================================================
 *
 * definitions about tagged word
 *
 *==============================================================*/

// the word class, which includes hash
template <typename CTag, char sTagSep>
class CTaggedWord {

public:
   const static char sSeparator = sTagSep;

public:
   CWord word;
   CTag tag;

public:
   CTaggedWord() {}
//   CTaggedWord(const std::string &s, const CTag t) : word(s), tag(t) { }
   CTaggedWord(const std::string &s, const std::string &t) : word(s), tag(t) { }
   CTaggedWord(const CWord &w, const CTag &t) : word(w), tag(t) { }
   CTaggedWord(const CTaggedWord &w) : word(w.word), tag(w.tag) { }
   virtual ~CTaggedWord() {}

public:
   inline bool operator == (const CTaggedWord &w) const {
      return word == w.word && tag == w.tag ;
   }
   inline bool operator != (const CTaggedWord &w) const {
      return !(*this == w);
   }
   // the ordering of words are defined:
   // when the hash are not equal, order is defined by hash
   // when the hash are equal, order is defined by char-value
   inline bool operator < (const CTaggedWord &w) const {
      return word == w.word ? tag < w.tag : word < w.word ;
   }
   inline bool empty() { return word.empty() && tag.empty(); }
   inline void clear() { word.clear(); tag.clear(); }
   inline unsigned long hash() const { return (word.code()<<CTag::SIZE)|tag.code(); }
   // assign value
   inline void operator = (const CTaggedWord &tw) {
      word = tw.word;
      tag = tw.tag;
   }
   inline void load(const CWord &word, const CTag &tt=CTag::NONE) {
      this->word = (word) ;
      tag = tt ;
   }
};

//===============================================================

template <typename CTag, char sTagSep>
std::istream & operator >> (std::istream &is, CTaggedWord<CTag, sTagSep> &tw) {
   std::string s;
   is >> s;
   int middle;
   middle = s.rfind(sTagSep, s.size()-1);
   assert(middle>=0);
   assert(middle<static_cast<int>(s.size())-1);
   tw.load(CWord(s.substr(0,middle)), CTag(s.substr(middle+1)));
   return is ;
}

template <typename CTag, char sTagSep>
std::ostream & operator << (std::ostream &os, const CTaggedWord<CTag, sTagSep> &tw) {
   os << tw.word.str() ;
   os << sTagSep ;
   os << tw.tag.str() ;
   return os ;
}


#endif

