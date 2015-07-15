// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * utils.h - the utilities                                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H 1

#include "definitions.h"

/*===============================================================
 *
 * Array utilities
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * clearArray - clear an arbitrary two-dimenstional array x
 *
 *--------------------------------------------------------------*/

template <typename T>
inline void clearArray(T *x, const int X, const int Y) {
   for (int __temp_i=0; __temp_i<X; ++__temp_i)
      for (int __temp_j=0; __temp_j<Y; ++__temp_j)
         x[__temp_i*Y+__temp_j] = 0;
}

/*---------------------------------------------------------------
 *
 * printArray - print array
 *
 *--------------------------------------------------------------*/

template<typename T>
inline
void printArray(T *array, const int &size) {
   for (int i=0; i<size; ++i)
      std::cerr<<array[i]<<"\t";
   std::cerr<<std::endl;
}

/*===============================================================
 *
 * Vector utilities
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * insertNoduplicateIntoVector - insert a value into std::vector
 *                               if the value is in the std::vector, no change
 *
 *--------------------------------------------------------------*/

template<typename VE, typename V>
inline
void insertNoduplicateIntoVector(VE &vector, const V &value) {
   typename VE::iterator it;
   for(it=vector.begin(); it!=vector.end(); it++)
      if ((*it) == value)
         return;
   vector.push_back(value);
}

/*--------------------------------------------------------------
 *
 * toString - print each element std::vector
 *
 *-------------------------------------------------------------*/

template<typename VE>
inline
std::string toString(const std::vector<VE> &v) {
   std::ostringstream os;
   os << "[ ";
   for( unsigned long i=0; i<v.size(); ++i ) {
      os << v[i] << " ";
   }
   os << "]";
   return os.str();
}

/*===============================================================
 *
 * std::string utilities
 *
 *==============================================================*/

/*--------------------------------------------------------------
 *
 * fromString
 *
 *--------------------------------------------------------------*/

template <class T>
bool fromString(T& t,
                 const std::string& s,
                 std::ios_base& (*f)(std::ios_base&)=std::dec)
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

/*--------------------------------------------------------------
 *
 * lstrip
 *
 *--------------------------------------------------------------*/

inline std::string lstrip(const std::string &s) {
   unsigned index=0;
   while (index<s.size() && (s[index]==' ' || s[index]=='\n' || s[index]=='\r' || s[index]=='\t'))
      ++index;
   return s.substr(index);
}

/*--------------------------------------------------------------
 *
 * rstrip
 *
 *--------------------------------------------------------------*/

inline std::string rstrip(const std::string &s) {
   int index=s.size()-1;
   while (index>=0 && (s[index]==' ' || s[index]=='\n' || s[index]=='\r' || s[index]=='\t'))
      --index;
   return s.substr(0, index+1);
}

#endif
