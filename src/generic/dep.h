// Copyright (C) University of Oxford 2010
#ifdef LABELED
#include "dependency/label/generic.h"
#endif
namespace generic {
#include "dependency/rules/generic.h"
}

#ifdef LABELED
inline std::istream & operator >> (std::istream &is, generic::CDependencyLabel &label) {
   std::string s;
   is >> s;
   label.load(s);
   return is;
}

inline std::ostream & operator << (std::ostream &os, const generic::CDependencyLabel &label) {
   os << label.str() ;
   return os;
}

#endif

