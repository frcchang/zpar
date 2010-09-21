#ifndef _ENGLISH_DEPLABEL_H
#define _ENGLISH_DEPLABEL_H

#include "linguistics/dependency/penn.h"

//=============================================================

inline istream & operator >> (istream &is, CDependencyLabel &label) {
   string s;
   is >> s;
   label.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const CDependencyLabel &label) {
   os << label.str() ;
   return os;
}
#endif
