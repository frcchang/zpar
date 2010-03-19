/****************************************************************
 *                                                         
 * test.cpp 
 *                                                          
 * Author: Yue Zhang                                         
 *                                                        
 * Computing Laboratory, Oxford. 2006.10                 
 *                                                      
 ****************************************************************/

#include "options.h"

int main(int argc, char**argv){
   int c = 1;
   int d = 2;
   pair<int, int> a=make_pair(c, d);
   pair<const int&, const int&>b=make_pair(c, d);
   
   cout << (a==b) << endl;
   return 0;
};

