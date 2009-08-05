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
   COptions options(argc, argv);
   for (int i=0; i<options.args.size();++i)
      cout << options.args[i] << endl;
   for (int i=0; i<options.opts.size(); ++i)
      cout << options.opts[i].first << " " << options.opts[i].second << endl;
   return 0;
};

