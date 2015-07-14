// Copyright (C) University of Oxford 2010

/****************************************************************
 *                                                              *
 * definitions.h - the general definitions                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MACROTOSTRING(M) #M

/*===============================================================
 *
 * The things that should be included
 *
 *==============================================================*/

#include <cstdlib>                              // define NULL
#include <utility>                              // define std::pair
#include <cassert>                              // assert
#include <ctime>                                // clock(); CLK_TCK
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>
#include <cmath>

//using namespace std;

/*===============================================================
 *
 * debugging utilities
 *
 *==============================================================*/

// throw
#define THROW(x) { std::ostringstream __tmp_os; __tmp_os << x; throw(__tmp_os.str()); }

// assert
#define ASSERT(x,y) { if (!(x)) THROW(y); }

// error report tool
#define REPORT(x) { std::cerr << std::endl << "In " << __FILE__ << ", line " << __LINE__ << ": " << std::endl << x << std::endl; std::cerr.flush(); }
#define WARNING(x) { std::cerr << "Warning:" << " " << x << std::endl; std::cerr.flush(); }

// trace tool
#ifdef DEBUG
#define TRACE(x) { std::cerr << x << std::endl; std::cerr.flush(); }
#define TRACE_WORD(x) { std::cerr << x; std::cerr.flush(); }
#define TRACE_IF(x, y) { if (x) { std::cerr << y << std::endl; std::cerr.flush(); } }
#else
#define TRACE(x)
#define TRACE_WORD(x)
#define TRACE_IF(x, y)
#endif

// conditional compiling
#ifdef DEBUG
#define WHEN_DEBUGGING(x) x
#else
#define WHEN_DEBUGGING(x)
#endif

/*===============================================================
 *
 * definitions about sentence structures
 *
 *==============================================================*/

#include "linguistics/sentence_string.h"
#include "zinttypes.h"

#endif

