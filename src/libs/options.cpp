/****************************************************************
 *                                                              *
 * options.cpp - command line options                           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2009.7                         *
 *                                                              *
 ****************************************************************/

#include "options.h"

COptions::COptions(int argc, char* args[], const string& char_options, const vector<string>& str_options) {
   for ( unsigned i=0; i<argc; ++i ) {
      string arg( args[i] );
      string name;
      string value;
      if ( arg[0] == '-' ) {
         if ( arg.size() == 1 ) {
            valid = false;
            err = "invalid option: -";
            return;
         }
         if ( arg[1] == '-' ) { // --name value
            if ( arg.size() == 2 ) {
               valid = false;
               err = "invalid option: --";
               return;
            }
            name = arg.substr(2);
            value = args[++i];
         }
         else { // -[name]value
            name = arg[1];
            value = arg.substr(2);
         }
         opts.push_back(make_pair(name, value));
      }
      else {
         (*this).args.push_back(arg);
      }
   } // for
   valid = true;
}
      
