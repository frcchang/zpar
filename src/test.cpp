// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                         
 * test.cpp 
 *                                                          
 * Author: Yue Zhang                                         
 *                                                        
 * Computing Laboratory, Oxford. 2006.10                 
 *                                                      
 ****************************************************************/
#define SIMPLE_HASH
#include "definitions.h"
#include "table2.h"

class A {
public:
   enum {C=0, D};
};

class B : public A {
public:
   enum {C=A::C+1};
};

void testtable2() {
   CTable2<string, string, double> table2;
   ifstream is("./input.txt");
   is >> table2;
   cout << table2.lookup("'m", "!edge") << endl;
   cout << table2.lookup("OOV", "OOV") << endl;
   cout << table2.lookup("None", "1") << endl;
//   cout << table2;
   is.close();
}

int main(int argc, char**argv){
   try {
   cout << B::C << B::D << endl;
   cout << ~0UL << endl;
   testtable2();
   }catch(const string &s) { cout << s << endl; }
};

