// Copyright (C) University of Oxford 2010
#ifndef _TAGGER_MACROS_H
#define _TAGGER_MACROS_H
#define SIMPLE_HASH

namespace TARGET_LANGUAGE {

namespace tagger {

typedef double SCORE_TYPE ;

//
// Specific global
//
const int AGENDA_SIZE = 16;

//
// General definitions for any method tagger.
//
const int MAX_SENTENCE_SIZE = 512; // restrict size of sentences 

};

}

#endif
