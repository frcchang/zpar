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
   for (int i=0; i<size; i++)
      cout<<array[i]<<"\t";
   cout<<endl;
}

/*===============================================================
 *
 * Vector utilities
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * insertNoduplicateIntoVector - insert a value into vector
 *                               if the value is in the vector, no change
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
 * toString - print each element vector
 *
 *-------------------------------------------------------------*/

template<typename VE>
inline
string toString(const vector<VE> &v) {
   ostringstream os;
   os << "[ ";
   for( unsigned i=0; i<v.size(); ++i ) {
      os << v[i] << " ";
   }
   os << "]";
   return os.str();
}

/*===============================================================
 *
 * string utilities
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

#endif
