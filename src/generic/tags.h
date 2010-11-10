// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tags.h - the definitions for generic tags                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2010.04                        *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_TAGS_H
#define _GENERIC_TAGS_H 1

#include "pos/generic.h"

namespace generic {

const char TAG_SEPARATOR = '/';

}; // namespace generic

//===============================================================

inline std::istream & operator >> (std::istream &is, generic::CTag &tag) {
   std::string s;
   is >> s;
   tag.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const generic::CTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

