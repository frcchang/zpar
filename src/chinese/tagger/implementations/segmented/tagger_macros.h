// Copyright (C) University of Oxford 2010
#ifndef _CHINESE_TAGGER_MACROS_H
#define _CHINESE_TAGGER_MACROS_H
#define SIMPLE_HASH
#define SEGMENTED 1
namespace chinese {
namespace tagger {
typedef double SCORE_TYPE ;
//
// Specific global
//
const int AGENDA_SIZE = 16;
//
// General definitions for any method tagger.
//
const int MAX_SENTENCE_SIZE = 1024; // restrict size of sentences 
}} // namespaces
#endif
