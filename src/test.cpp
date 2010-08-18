/****************************************************************
 *                                                         
 * test.cpp 
 *                                                          
 * Author: Yue Zhang                                         
 *                                                        
 * Computing Laboratory, Oxford. 2006.10                 
 *                                                      
 ****************************************************************/

#include "definitions.h"

class A {
public:
   enum {C=0, D};
};

class B : public A {
public:
   enum {C=A::C+1};
};

int main(int argc, char**argv){
   cout << B::C << B::D << endl;
   cout << ~0UL << endl;
};

