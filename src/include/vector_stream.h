// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * vector_stream.h - this module defines the stream io utility  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _VECTOR_STREAM_UTILITIES_H
#define _VECTOR_STREAM_UTILITIES_H 

template<typename T>
istream & operator >> (istream &is, vector<T> &p) {
   char c ;
   is >> c;
   ASSERT(c=='[', "Incorrectly formed vector, missing [");
   p.clear();
   T t;
   while (is>>t) {
      p.push_back(t);
      is >> c;
      if (c==']')
         return is;
      ASSERT(c==',', "Incorrectly formed vector, missing ,");
   }
   THROW("Incorrectly formed vector, missing ]");
}

template<typename T>
ostream & operator << (ostream &os, const vector<T> &p) {
   os << "[ ";
   for (int i=0; i<p.size(); ++i) {
      os << p.at(i);
      if (i!=p.size()-1) os << " , ";
   }
   os << " ]";
   return os ;
}

#endif
