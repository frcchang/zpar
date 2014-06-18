// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * agenda.cpp - the agenda pos tagger implementation.           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#include <iostream>
#include <cstdlib>
#include "tagger.h"

using namespace std;
using namespace TARGET_LANGUAGE;

bool TARGET_LANGUAGE::CTagger::train(const CTwoStringVector *correct)
{
  cerr << "Training not supported inside zpar." << endl;
  exit(-1);
}
