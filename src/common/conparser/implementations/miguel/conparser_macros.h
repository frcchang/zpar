// Copyright (C) University of Oxford 2010
#ifndef _GENERAL_CONPARSER_MACROS_H
#define _GENERAL_CONPARSER_MACROS_H

#define SIMPLE_HASH

//#define NO_NEG_FEATURE //THESE FOR GOLD STANDARD TESTING remove!! //FOR GOLD-STD

// using passive aggressive
//#define TRAIN_MARGIN

// applying loss func
//#define TRAIN_LOSS

// using mira
//#define TRAIN_MULTI

// early update? 
#define EARLY_UPDATE

// scale scores? this must be used with TRAIN_MARGIN or undefined
//#define SCALE

#define STANFORD_DEPENDENCIES

#define CONLL_OUTPUT

#define TRACE_STANFORD

// The size of agenda
static const unsigned long AGENDA_SIZE = 32;
static const unsigned long MIRA_SIZE = AGENDA_SIZE+1;

// The size of a sentence and the words
static const int MAX_SENTENCE_SIZE = 256 ; 
static const int MAX_SENTENCE_SIZE_BITS = 8 ; 

// normalize numberings
inline unsigned long normalize3(const unsigned long &x) {
   assert(x!=0); return x>2 ? 2 : x==2 ? 1 : 0;
}

inline unsigned long normalize16(const unsigned long &x) {
   return x>15 ? 15 : x;
}

inline unsigned long normalize510(const unsigned long &x) {
   return x>10 ? 6 : x>5 ? 5 : x;
}

// unary moves 
const int UNARY_MOVES = 3;

const unsigned long HEAD_LEFT = 1;
const unsigned long HEAD_RIGHT = 2;
inline unsigned long encodeLinkDirection(const unsigned long &head, const unsigned long &mod) {
   assert(mod!=head);
   return mod>head ? HEAD_LEFT : HEAD_RIGHT;
}

inline unsigned long encodeLinkSize(const unsigned long &head, const unsigned long &mod) {
   assert(mod!=head);
   return mod>head ? normalize510(mod-head) : normalize510(head-mod);
}

inline unsigned long encodeLinkDirectionAndSize(const unsigned long &head, const unsigned long &mod) {
   assert(mod!=head);
   assert((normalize510(mod>head?mod-head:head-mod)>>3)==0);
   if (mod>head)
      return (HEAD_LEFT<<3) | normalize510(mod-head);
   else
      return (HEAD_RIGHT<<3) | normalize510(head-mod);
}

#ifndef TRAIN_MARGIN
#undef SCALE
#endif

// Yue: for Stanford Links. Nov. 2013

// link size and direction are combined
const int LINK_DIRECTION_HEAD_LEFT = -7 ; // head on the left
const int LINK_DIRECTION_HEAD_RIGHT = 7 ; // on the right

// normalise link size and the direction
inline int getLinkSizeAndDirection(const int &head_index, const int &dep_index) {
   static int diff;
   diff = head_index - dep_index;
   assert(diff != 0); 
   if (diff>10) diff = 6; 
   else if (diff>5) diff = 5; 
   if (diff<-10) diff = -6; 
   else if (diff<-5) diff = -5;
   return diff;
}
inline int getLinkDirection(const int &head_index, const int &dep_index) {
   return head_index>dep_index ? 0 : 1 ;
}


#endif
