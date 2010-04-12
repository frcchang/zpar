#ifndef _GENERAL_DEPLABELER_MACROS_H
#define _GENERAL_DEPLABELER_MACROS_H

typedef double SCORE_TYPE ;

// The size of a sentence and the words
const int MAX_SENTENCE_SIZE = 256 ; 
const int MAX_SENTENCE_SIZE_BITS = 8 ; 

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
inline int getLinkDirectionEncode(const int &head_index, const int &dep_index) {
   return head_index>dep_index ? LINK_DIRECTION_HEAD_RIGHT : LINK_DIRECTION_HEAD_LEFT ;
}
inline void UnlabelSentence(const CLabeledDependencyTree &labeled, CDependencyTree &output) {
   static int i;
   output.clear();
   for (i=0;i<labeled.size();++i)
      output.push_back(CDependencyTreeNode(labeled[i].word, labeled[i].tag, labeled[i].head));
}

// arity direction
enum ARITY_DIRECTION { ARITY_DIRECTION_LEFT=0, ARITY_DIRECTION_RIGHT=1 } ;

#endif
