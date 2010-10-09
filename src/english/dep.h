#ifndef _ENGLISH_DEPLABEL_H
#define _ENGLISH_DEPLABEL_H

#include "dependency/rules/tmp.h"

#ifdef LABELED
#include "dependency/label/penn.h"
inline istream & operator >> (istream &is, english::CDependencyLabel &label) {
   string s;
   is >> s;
   label.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const english::CDependencyLabel &label) {
   os << label.str() ;
   return os;
}
#endif

#endif
