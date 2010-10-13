// Copyright (C) University of Oxford 2010
namespace chinese {

#ifdef LABELED
#include "dependency/label/ctb.h"
#endif
#include "dependency/rules/ctb.h"

}

//=============================================================

#ifdef LABELED
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
#endif

