// Copyright (C) University of Oxford 2010
#ifndef _GENERAL_DEPPARSER_MACROS_H
#define _GENERAL_DEPPARSER_MACROS_H

#define CHINESE_POS_SET pmt

#define CHINESE_DEP_SET pmt

#define CHINESE_CFG_SET pmt

#define SIMPLE_HASH

// early update? 
#define EARLY_UPDATE

// local training? it will affect the setting of early update and agenda size
// if you want to experiment with local training, define this when you train
// it will automatically set beam1
// but undefine this when you decode with beam more than one
// using the model you trained with this defined
//#define LOCAL_LEARNING

// The size of agenda
#define AGENDA_SIZE 64

typedef int64_t SCORE_TYPE ;

// supertag
#define SR_SUPERTAG 1

// force the tree to be single-rooted or allow multiple pseudo roots
//#define FRAGMENTED_TREE

// the implementation supports the extraction of features as a command
#define SUPPORT_FEATURE_EXTRACTION

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
