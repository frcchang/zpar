/****************************************************************
 *                                                              *
 * tags.h - the definitions for generic tags                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_TAGS_H
#define _GENERIC_TAGS_H 1

namespace generic {

#include "linguistics/generictag.h"

const char TAG_SEPARATOR = '/';

}; // namespace generic

//===============================================================

inline const unsigned long &hash(const generic::CTag &tag) { return tag.code();}

//===============================================================

inline istream & operator >> (istream &is, generic::CTag &tag) {
   string s;
   is >> s;
   tag.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const generic::CTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

