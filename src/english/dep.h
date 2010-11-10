// Copyright (C) University of Oxford 2010
#ifndef _ENGLISH_DEPLABEL_H
#define _ENGLISH_DEPLABEL_H

#include "dependency/rules/penn.h"
//#include "dependency/rules/stanford.h"

#ifdef LABELED
#include "dependency/label/penn.h"
//#include "dependency/label/stanford.h"
inline std::istream & operator >> (std::istream &is, english::CDependencyLabel &label) {
   std::string s;
   is >> s;
   label.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const english::CDependencyLabel &label) {
   os << label.str() ;
   return os;
}
#endif

#endif
