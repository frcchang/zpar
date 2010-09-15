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
   CTable2<string, int, double> table2;
   ifstream is("input.txt");
//   is >> table2;
   cout << table2.lookup("One", 1);
   cout << table2.lookup("Two", 3);
   cout << table2.lookup("None", 1);
//   cout << table2;
   is.close();
}

int main(int argc, char**argv){
   cout << B::C << B::D << endl;
   cout << ~0UL << endl;
};

