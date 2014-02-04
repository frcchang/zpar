// Copyright (C) University of Oxford 2010
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
#define _CHINESE_TAGS_H

#ifndef CHINESE_POS_SET
#define CHINESE_POS_SET ctb5
#endif

#define INCLUDE_FILE(M) MACROTOSTRING(pos/M.h)
#include INCLUDE_FILE(CHINESE_POS_SET)
#undef INCLUDE_FILE

namespace chinese {

const char TAG_SEPARATOR = '_';

}; // namespace chinese

//===============================================================

//inline const unsigned long &hash(const chinese::CTag &tag) { return tag.code();}

//===============================================================

inline std::istream & operator >> (std::istream &is, chinese::CTag &tag) {
   std::string s;
   is >> s;
   tag.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const chinese::CTag &tag) {
   os << tag.str() ;
   return os;
}

#endif

