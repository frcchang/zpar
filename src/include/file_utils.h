// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * file_utils.h - the file utilities                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _FILE_UTILITIES_H
#define _FILE_UTILITIES_H 

#include <sys/stat.h>

inline
bool FileExists(const std::string &strFilename) {
  struct stat stFileInfo;
  bool blnReturn;
  int intStat;

  // Attempt to get the file attributes
  intStat = stat(strFilename.c_str(),&stFileInfo);
  if(intStat == 0) {
     // We were able to get the file attributes
     // so the file obviously exists.
     blnReturn = true;
  } else {
     // We were not able to get the file attributes.
     // This may mean that we don't have permission to
     // access the folder which contains this file. If you
     // need to do that level of checking, lookup the
     // return values of stat which will give you
     // more details on why stat failed.
     blnReturn = false;
  }
  
  return(blnReturn);
}

#endif
