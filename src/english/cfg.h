// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * cfg.h - the definitions for english cfg                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _ENGLISH_CFG_H
#define _ENGLISH_CFG_H

#include "tags.h"
#include "cfg/penn.h"

//===============================================================

//inline unsigned long hash(const english::CConstituentLabel &con) { return con.code();}

//===============================================================

inline istream & operator >> (istream &is, english::CConstituentLabel &con) {
   string s;
   is >> s;
   con.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const english::CConstituentLabel &con) {
   os << con.str() ;
   return os;
}

#endif

