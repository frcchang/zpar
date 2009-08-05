#ifndef _GENERAL_CONPARSER_MACROS_H
#define _GENERAL_CONPARSER_MACROS_H

// early update? 
#define EARLY_UPDATE 1


// The size of agenda
const int AGENDA_SIZE = 16 ;

// The size of a sentence and the words
const int MAX_SENTENCE_SIZE = 256 ; 
const int MAX_SENTENCE_SIZE_BITS = 8 ; 

inline unsigned encodeT(const CTaggedWord<CTag> &wd) {
   assert(PENN_TAG_COUNT+7<(1<<PENN_TAG_COUNT_BITS));
   const unsigned t = wd.tag.code();
return t;
   if (t==PENN_TAG_PU) {
      const CWord &w = wd;
      if (w==CH_COMMA)
         return PENN_TAG_COUNT;
      if (w==CH_PERIOD)
         return PENN_TAG_COUNT+1;
      if (w==CH_DUN)
         return PENN_TAG_COUNT+2;
      if (w==CH_SEMICOLON)
         return PENN_TAG_COUNT+3;
      if (w==CH_COLON)
         return PENN_TAG_COUNT+4;
      if (getStartingBracket(w)!=-1)
         return PENN_TAG_COUNT+5;
      if (getEndingBracket(w)!=-1)
         return PENN_TAG_COUNT+6;
   }
   return t;
}

inline unsigned encodeC(const CStateNode &node, const vector<CTaggedWord<CTag> > &cache ) {
   assert((node.constituent&(1<<PENN_CON_COUNT_BITS))==0);
   return node.is_constituent ? encodeTorC(node.constituent, false) | (node.temp?(1<<PENN_CON_COUNT_BITS):0) : encodeTorC(encodeT(cache[node.lexical_head]), true);
}

inline unsigned normalize3(const unsigned &x) {
   assert(x!=0); return x>2 ? 2 : x==2 ? 1 : 0;
}

inline unsigned long normalize16(const unsigned long &x) {
   return x>15 ? 15 : x;
}

inline unsigned long normalize510(const unsigned long &x) {
   return x>10 ? 6 : x>5 ? 5 : x;
}

inline unsigned encodeRhythm(const CStateNode &node, const vector<unsigned> &wordlen) {
   // single word consitituent : 1wordlen; 
   // multiple word constituent : 0wordnum;
   assert(node.lexical_start!=-1&&node.lexical_end!=-1&&node.lexical_head!=-1);
   return (node.lexical_start==node.lexical_end ? wordlen[node.lexical_head]|(1<<2) : normalize3(node.lexical_end-node.lexical_start));
}

inline unsigned encodeRhythms(const unsigned &r1, const unsigned &r2) {
   assert(r2>>3==0);
   return (r1<<3)|r2;
}

// unary moves 
const int UNARY_MOVES = 3;

const unsigned HEAD_LEFT = 1;
const unsigned HEAD_RIGHT = 2;
inline unsigned encodeLinkDirection(const unsigned &head, const unsigned &mod) {
   assert(mod!=head);
   return mod>head ? HEAD_LEFT : HEAD_RIGHT;
}

inline unsigned encodeLinkSize(const unsigned &head, const unsigned &mod) {
   assert(mod!=head);
   return mod>head ? normalize510(mod-head) : normalize510(head-mod);
}

inline unsigned encodeLinkDirectionAndSize(const unsigned &head, const unsigned &mod) {
   assert(mod!=head);
   assert((normalize510(mod>head?mod-head:head-mod)>>3)==0);
   if (mod>head)
      return (HEAD_LEFT<<3) | normalize510(mod-head);
   else
      return (HEAD_RIGHT<<3) | normalize510(head-mod);
}

#endif
