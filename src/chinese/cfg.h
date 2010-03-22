/****************************************************************
 *                                                              *
 * cfg.h - the definitions for chinese cfg                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_CFG_H
#define _CHINESE_CFG_H

#include "tags.h"
#include "cfg/penn.h"

namespace chinese {

const unsigned long PENN_CON_TAG_COUNT_BITS = std::max(static_cast<unsigned long>(CConstituent::SIZE), static_cast<unsigned long>(CTag::SIZE)) + 1;

inline unsigned long encodeConstituents(const unsigned long &con1, const unsigned long &con2) {
   return (con1<<CConstituent::SIZE) | con2;
}
inline unsigned long int encodeConstituents(const unsigned long &con1, const unsigned long &con2, const unsigned long &con3) {
   return (con1<<CConstituent::SIZE*2) | (con2<<CConstituent::SIZE) | con3 ;
}
inline unsigned long int encodeConstituents(const unsigned long &con1, const unsigned long &con2, const unsigned long &con3, const unsigned long &con4) {
   return (con1<<CConstituent::SIZE*3) | (con2<<CConstituent::SIZE*2) |
          (con3<<CConstituent::SIZE) | con4 ;
}

inline unsigned long int encodeConstituents(const CConstituent &con1, const CConstituent &con2) {
   return (con1.code()<<CConstituent::SIZE) | con2.code();
}
inline unsigned long int encodeConstituents(const CConstituent &con1, const CConstituent &con2, const CConstituent &con3) {
   return (con1.code()<<CConstituent::SIZE*2) | (con2.code()<<CConstituent::SIZE) | con3.code() ;
}
inline unsigned long int encodeConstituents(const CConstituent &con1, const CConstituent &con2, const CConstituent &con3, const CConstituent &con4) {
   return (con1.code()<<CConstituent::SIZE*3) | (con2.code()<<CConstituent::SIZE*2) |
          (con3.code()<<CConstituent::SIZE) | con4.code() ;
}

}; // namespace chinese

//===============================================================

inline unsigned long hash(const chinese::CConstituent &con) { return con.code();}

//===============================================================

inline istream & operator >> (istream &is, chinese::CConstituent &con) {
   string s;
   is >> s;
   con.load(s, false);
   return is;
}

inline ostream & operator << (ostream &os, const chinese::CConstituent &con) {
   os << con.str() ;
   return os;
}

#endif
