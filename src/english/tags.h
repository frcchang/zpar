/****************************************************************
 *                                                              *
 * tags.h - the definitions for english tags                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _ENGLISH_TAGS_H
#define _ENGLISH_TAGS_H

#include "pos/penn.h"

namespace english {

const char TAG_SEPARATOR = '/';

}; // namespace english

//===============================================================

inline unsigned long hash(const english::CTag &tag) { return tag.code();}

//===============================================================

inline istream & operator >> (istream &is, english::CTag &tag) {
   string s;
   is >> s;
   tag.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const english::CTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

