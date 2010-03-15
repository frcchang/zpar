/****************************************************************
 *                                                              *
 * tags.h - the definitions for chinese tags                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_TAGS_H
#define _CHINESE_TAGS_H 1

//#include <string>
#include "pos/penn.h"

namespace chinese {

const char TAG_SEPARATOR = '_';

}; // namespace chinese

//===============================================================

inline const unsigned long &hash(const chinese::CTag &tag) { return tag.code();}

//===============================================================

inline istream & operator >> (istream &is, chinese::CTag &tag) {
   string s;
   is >> s;
   tag.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const chinese::CTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

