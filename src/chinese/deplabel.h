#include "linguistics/dependency/penn.h"

//=============================================================

inline istream & operator >> (istream &is, chinese::CDependencyLabel &label) {
   string s;
   is >> s;
   label.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const chinese::CDependencyLabel &label) {
   os << label.str() ;
   return os;
}

