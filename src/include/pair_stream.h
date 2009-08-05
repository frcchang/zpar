/****************************************************************
 *                                                              *
 * pair_stream.h - this module defines the stream io utilities  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.2                         *
 *                                                              *
 ****************************************************************/

#ifndef _PAIR_STREAM_UTILITIES_H
#define _PAIR_STREAM_UTILITIES_H 

template<typename T1, typename T2>
istream & operator >> (istream &is, pair<T1, T2> &p) {
   char c ;
   is >> p.first >> c >> p.second ;
   assert(c==',');
   return is ;
}

template<typename T1, typename T2>
ostream & operator << (ostream &os, const pair<T1, T2> &p) {
   os << p.first << " , " << p.second ;
   return os ;
}

#endif
