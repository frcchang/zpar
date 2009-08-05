/****************************************************************
 *                                                              *
 * options.h - command line options reading utility.            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "definitions.h"

/*===============================================================
 *
 * COptions - the command line options
 *
 *==============================================================*/

class COptions {

public:

   vector< string > args;
   vector< pair< string, string > > opts;

   bool valid;
   string err;

   COptions(int argc, char* args[], const string& char_options="", const vector<string>& str_options=vector<string>());

};

#endif
