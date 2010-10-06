#ifdef LABELED
#include "dependency/label/generic.h"
#endif

#include "dependency/rules/generic.h"

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

