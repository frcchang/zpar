// Copyright (C) University of Oxford 2010
#ifndef _HASH_FUNCTION_UTIL_H
#define _HASH_FUNCTION_UTIL_H

#include "definitions.h"

// The idea here is to provide a global hash function for basic types
// but for all types defined in my code they are give a hash() member
// so that the inline hash functions are always there.

inline unsigned long hash(const unsigned int &i) {return i;}
inline unsigned long hash(const int &i) {return i;} //return i>0?i:-i;}

inline const unsigned long &hash(const unsigned long &i) {return i;}
inline unsigned long hash(const long &i) {return i;} //return i>0?i:-i;}

inline unsigned long hash(const long long &i) {return i;} //return i>0?i:-i;}
inline unsigned long hash(const unsigned long long &i) {return i;}

inline unsigned long hash(const std::string &s) {unsigned long retval=0; for (unsigned long i=0; i<s.size(); ++i) retval=retval*33+(static_cast<unsigned char>(s[i])&127); return retval;}

inline unsigned long hash(const bool &b) {return b?1ul:0ul;}

template<typename T>
inline unsigned long hash(const T& t) { return t.hash(); }

template<typename T1, typename T2>
inline unsigned long hash(const std::pair<T1, T2> &o) { return hash(o.first)*33 + hash(o.second) ; }

#endif
