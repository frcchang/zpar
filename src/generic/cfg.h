// Copyright (C) University of Oxford 2010
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
   return (con1<<CConstituentLabel::SIZE) | con2;
}
inline unsigned long int encodeConstituents(const unsigned long &con1, const unsigned long &con2, const unsigned long &con3) {
   return (con1<<CConstituentLabel::SIZE*2) | (con2<<CConstituentLabel::SIZE) | con3 ;
}
inline unsigned long int encodeConstituents(const unsigned long &con1, const unsigned long &con2, const unsigned long &con3, const unsigned long &con4) {
   return (con1<<CConstituentLabel::SIZE*3) | (con2<<CConstituentLabel::SIZE*2) |
          (con3<<CConstituentLabel::SIZE) | con4 ;
}

inline unsigned long int encodeConstituents(const CConstituentLabel &con1, const CConstituentLabel &con2) {
   return (con1.code()<<CConstituentLabel::SIZE) | con2.code();
}
inline unsigned long int encodeConstituents(const CConstituentLabel &con1, const CConstituentLabel &con2, const CConstituentLabel &con3) {
   return (con1.code()<<CConstituentLabel::SIZE*2) | (con2.code()<<CConstituentLabel::SIZE) | con3.code() ;
}
inline unsigned long int encodeConstituents(const CConstituentLabel &con1, const CConstituentLabel &con2, const CConstituentLabel &con3, const CConstituentLabel &con4) {
   return (con1.code()<<CConstituentLabel::SIZE*3) | (con2.code()<<CConstituentLabel::SIZE*2) |
          (con3.code()<<CConstituentLabel::SIZE) | con4.code() ;
}

}; // namespace generic

//===============================================================

//inline unsigned long hash(const generic::CConstituentLabel &con) { return con.code();}

//===============================================================

inline std::istream & operator >> (std::istream &is, generic::CConstituentLabel &con) {
   std::string s;
   is >> s;
   if (!is) {
      ASSERT(s.empty(), "something was read when is null");
      return is;
   }
   con.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const generic::CConstituentLabel &con) {
   os << con.str() ;
   return os;
}

#endif

