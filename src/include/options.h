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
   std::vector< std::string > args;
   std::vector< std::pair< std::string, std::string > > opts;

   bool valid;
   std::string err;

public:
   COptions(int argc, char* args[], const std::string& char_options="", const std::vector<std::string>& str_options=std::vector<std::string>());

};

/*===============================================================
 *
 * CConfigurations - the configuration
 *
 *==============================================================*/

class CConfigurations {

public:
   struct CCon {
      std::string name;
      std::string argument;
      std::string message;
      std::string value;
      CCon(const std::string &name, const std::string &argument, const std::string &message, const std::string &value) : name(name), argument(argument), message(message), value(value) {};
   };
   std::vector< CCon > configurations;

public:
   CConfigurations() {};
   void defineConfiguration(const std::string &name, const std::string &argument, const std::string &type, const std::string &message);
   // retval: error message
   std::string loadConfigurations(const std::vector< std::pair< std::string, std::string > > &options);
   std::string getConfiguration(const std::string &name);
   std::string message();

};

#endif
