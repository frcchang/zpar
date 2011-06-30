// Copyright (C) University of Oxford 2010
#ifndef _GENERAL_DEPPARSER_MACROS_H
#define _GENERAL_DEPPARSER_MACROS_H

#define SIMPLE_HASH

// early update? 
#define EARLY_UPDATE

// local training? it will affect the setting of early update and agenda size
#define LOCAL_LEARNING

// The size of agenda
#define AGENDA_SIZE 64

//label
#ifdef LABELED
const unsigned DEP_LABEL_COUNT=CDependencyLabel::MAX_COUNT;
#else
const unsigned DEP_LABEL_COUNT = 1;
#endif

typedef long SCORE_TYPE ;
#include "action.h"

// supertag
#define SR_SUPERTAG 1

// The size of a sentence and the words
const unsigned MAX_SENTENCE_SIZE = 256 ; 
const unsigned MAX_SENTENCE_SIZE_BITS = 8 ; 

// normalise link size and the direction
inline int encodeLinkDistance(const int &head_index, const int &dep_index) {
   static int diff;
   diff = head_index - dep_index;
   assert(diff != 0); 
   if (diff<0)
      diff=-diff;
   if (diff>10) diff = 6; 
   else if (diff>5) diff = 5; 
   return diff;
}

// arity direction
enum ARITY_DIRECTION { ARITY_DIRECTION_LEFT=0, ARITY_DIRECTION_RIGHT=1 } ;

#ifdef LOCAL_LEARNING
#define EARLY_UPDATE
#define AGENDA_SIZE 1
#endif

#endif
