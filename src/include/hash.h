// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * hash.h - the hash table                                      *
 *                                                              *
 * There are different implementations of the hash table.       *
 * Currently we are using a hash map with tree nodes.           *
 * Later we may switch to the unordered map of C++ if that      *
 * is available for the platform.                               *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

// utilities defining the hash for types
#include "hash_utils.h"
// the implementation: choose between hash_simple and hash_combined
#include "hash_simple.h"
//#include "hash_combined.h"
//#endif

// io
#include "hash_stream.h"
