// Copyright (C) University of Oxford 2010
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

public:
   COptions(int argc, char* args[], const string& char_options="", const vector<string>& str_options=vector<string>());

};

/*===============================================================
 *
 * CConfigurations - the configuration
 *
 *==============================================================*/

class CConfigurations {

public:
   struct CCon {
      string name;
      string argument;
      string message;
      string value;
      CCon(const string &name, const string &argument, const string &message, const string &value) : name(name), argument(argument), message(message), value(value) {};
   };
   vector< CCon > configurations;

public:
   CConfigurations() {};
   void defineConfiguration(const string &name, const string &argument, const string &type, const string &message);
   // retval: error message
   string loadConfigurations(const vector< pair< string, string > > &options);
   string getConfiguration(const string &name);
   string message();

};

#endif
