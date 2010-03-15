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

inline unsigned long encodeTags(const unsigned long &tag1, const unsigned long &tag2) {
   assert((tag2>>CTag::SIZE)==0);
   return (tag1<<CTag::SIZE) | tag2;
}
inline unsigned long encodeTags(const unsigned long &tag1, const unsigned long &tag2, const unsigned long &tag3) {
   return (tag1<<CTag::SIZE*2) | (tag2<<CTag::SIZE) | tag3 ;
}
inline unsigned long encodeTags(const unsigned long &tag1, const unsigned long &tag2, const unsigned long &tag3, const unsigned long &tag4) {
   return (tag1<<CTag::SIZE*3) | (tag2<<CTag::SIZE*2) |
          (tag3<<CTag::SIZE) | tag4 ;
}

inline unsigned long encodeTags(const CTag &tag1, const CTag &tag2) {
   return (tag1.code()<<CTag::SIZE) | tag2.code();
}
inline unsigned long encodeTags(const CTag &tag1, const CTag &tag2, const CTag &tag3) {
   return (tag1.code()<<CTag::SIZE*2) | (tag2.code()<<CTag::SIZE) | tag3.code() ;
}
inline unsigned long encodeTags(const CTag &tag1, const CTag &tag2, const CTag &tag3, const CTag &tag4) {
   assert( CTag::SIZE*4<sizeof(unsigned long)*8 ); 
   return (tag1.code()<<CTag::SIZE*3) | (tag2.code()<<CTag::SIZE*2) |
          (tag3.code()<<CTag::SIZE) | tag4.code() ;
}

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

