/****************************************************************
 *                                                              *
 * cfg.h - the definitions for generic cfg                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_CFG_H
#define _GENERIC_CFG_H

#include "definitions.h"
#include "linguistics/cfg/generic.h"

namespace generic {

//===============================================================

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

}; // namespace generic

//===============================================================

//inline unsigned long hash(const generic::CConstituent &con) { return con.code();}

//===============================================================

inline istream & operator >> (istream &is, generic::CConstituent &con) {
   string s;
   is >> s;
   con.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const generic::CConstituent &con) {
   os << con.str() ;
   return os;
}

#endif

