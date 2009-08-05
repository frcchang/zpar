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
template <typename CTag>
class CTaggedWord : virtual public CWord {

public:
   CTag tag;

public:
   CTaggedWord() : CWord() { }
   CTaggedWord(const string &s, const CTag t) : CWord(s) { tag=t; }
   CTaggedWord(const CWord &w, const CTag t) : CWord(w) { tag=t; }
   CTaggedWord(const CTaggedWord &w) : CWord(static_cast<CWord>(w)) { tag = w.tag; }
   virtual ~CTaggedWord() {}

public:
   inline bool operator == (const CTaggedWord &w) const { 
      return static_cast<CWord>(*this) == static_cast<CWord>(w) && (tag == w.tag) ; 
   }
   inline bool operator != (const CTaggedWord &w) const { 
      return static_cast<CWord>(*this) != static_cast<CWord>(w) || (tag != w.tag); 
   }
   // the ordering of words are defined:
   // when the hash are not equal, order is defined by hash
   // when the hash are equal, order is defined by char-value
   inline bool operator < (const CTaggedWord &w) const { 
      return static_cast<CWord>(*this) == static_cast<CWord>(w) ?  
             tag < w.tag : 
             static_cast<CWord>(*this) < static_cast<CWord>(w) ; 
   }
   inline bool empty() { return static_cast<CWord>(*this).empty() && tag==0; }
   inline bool clear() { CWord::clear(); tag = 0; }
   // assign value
   inline void operator = (const CTaggedWord &word) {
      static_cast<CWord>(*this) = static_cast<const CWord &>(word);
      tag = word.tag;
   }
   inline void load(const CWord &word, const CTag &tt=0) {
      CWord::operator=(word) ;
      tag = tt ;
   }
};
   
//===============================================================

template <typename CTag>
inline unsigned long int hash( const CTaggedWord<CTag> &tw ) { return hash(static_cast<CWord>(tw))*37+hash(tw.tag); }

//===============================================================

template <typename CTag>
istream & operator >> (istream &is, CTaggedWord<CTag> &tw) {
   string s;
   is >> s;
   int middle;
   middle = s.rfind(CTag::SEPARATOR, s.size()-1);
   assert(middle>=0);
   assert(middle<s.size()-1);
   tw.load(CWord(s.substr(0,middle)), CTag(s.substr(middle+1)));
   return is ;
}

template <typename CTag>
ostream & operator << (ostream &os, const CTaggedWord<CTag> &tw) {
   os << tw.str() ;
   os << CTag::SEPARATOR ;
   os << tw.tag ; 
   return os ;
}


#endif

