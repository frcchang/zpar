/*
 * Spanish tags
 */

#ifndef _SPANISH_TAGS_H
#define _SPANISH_TAGS_H

#ifdef ES06_DEPENDENCIES
#include "pos/es06.h"
#else
#include "pos/es09.h"
#endif


namespace spanish {

const char TAG_SEPARATOR = '/';

}; // namespace spanish

//===============================================================

inline unsigned long hash(const spanish::CTag &tag) { return tag.code();}

//===============================================================

inline std::istream & operator >> (std::istream &is, spanish::CTag &tag) {
   std::string s;
   is >> s;
   tag.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const spanish::CTag &tag) {
   os << tag.str() ;
   return os;
}

//===============================================================

//#include "pos/penn_morph.h"

#endif

