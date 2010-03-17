#ifndef _HASH_FUNCTION_UTIL_H
#define _HASH_FUNCTION_UTIL_H

#include "definitions.h"

inline unsigned long hash(const unsigned int &i) {return i;}
inline unsigned long hash(const int &i) {return i>0?i:-i;}

inline unsigned long hash(const unsigned long &i) {return i;}
inline unsigned long hash(const long &i) {return i>0?i:-i;}

inline unsigned long hash(const long long &i) {return i>0?i:-i;}
inline unsigned long hash(const unsigned long long &i) {return i>0?i:-i;}

inline unsigned long hash(const string &s) {unsigned long retval=0; for (unsigned long i=0; i<s.size(); ++i) retval=retval*37+(static_cast<unsigned char>(s[i])&127); return retval;}

template<typename T1, typename T2>
inline unsigned long hash(const pair<T1, T2> &o) { return hash(o.first)*29 + hash(o.second) ; }

#endif
