/****************************************************************
 *                                                              *
 * stanford.h - the Stanford Dependency style dependency labels *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_L_STANFORD
#define _DEPENDENCY_L_STANFORD

#include "tags.h"

namespace english {

const string STANFORD_DEP_STRINGS[] = {
   "-none-",
   "null",
   "cc", 
   "number", 
   "ccomp", 
   "possessive", 
   "prt", 
   "num", 
   "nsubjpass", 
   "csubj", 
   "measure", 
   "conj", 
   "amod", 
   "pred", 
   "nn", 
   "neg", 
   "csubjpass", 
   "mark", 
   "auxpass", 
   "infmod", 
   "rel", 
   "advcl", 
   "aux", 
   "prep", 
   "parataxis", 
   "nsubj", 
   "rcmod", 
   "advmod", 
   "punct", 
   "quantmod", 
   "tmod", 
   "acomp", 
   "complm", 
   "pcomp", 
   "poss", 
   "xcomp", 
   "cop", 
   "attr", 
   "partmod", 
   "dep", 
   "appos", 
   "det", 
   "dobj", 
   "purpcl", 
   "abbrev", 
   "pobj", 
   "iobj", 
   "expl", 
   "predet", 
   "preconj"
};

enum STANFORD_DEP_LABELS {
   STANFORD_DEP_NONE=0,
   STANFORD_DEP_ROOT,
   STANFORD_DEP_CC, 
   STANFORD_DEP_NUMBER, 
   STANFORD_DEP_CCOMP, 
   STANFORD_DEP_POSSESSIVE, 
   STANFORD_DEP_PRT, 
   STANFORD_DEP_NUM, 
   STANFORD_DEP_NSUBJPASS, 
   STANFORD_DEP_CSUBJ, 
   STANFORD_DEP_MEASURE, 
   STANFORD_DEP_CONJ, 
   STANFORD_DEP_AMOD, 
   STANFORD_DEP_PRED, 
   STANFORD_DEP_NN, 
   STANFORD_DEP_NEG, 
   STANFORD_DEP_CSUBJPASS, 
   STANFORD_DEP_MARK, 
   STANFORD_DEP_AUXPASS, 
   STANFORD_DEP_INFMOD, 
   STANFORD_DEP_REL, 
   STANFORD_DEP_ADVCL, 
   STANFORD_DEP_AUX, 
   STANFORD_DEP_PREP, 
   STANFORD_DEP_PARATAXIS, 
   STANFORD_DEP_NSUBJ, 
   STANFORD_DEP_RCMOD, 
   STANFORD_DEP_ADVMOD, 
   STANFORD_DEP_PUNCT, 
   STANFORD_DEP_QUANTMOD, 
   STANFORD_DEP_TMOD, 
   STANFORD_DEP_ACOMP, 
   STANFORD_DEP_COMPLM, 
   STANFORD_DEP_PCOMP, 
   STANFORD_DEP_POSS, 
   STANFORD_DEP_XCOMP, 
   STANFORD_DEP_COP, 
   STANFORD_DEP_ATTR, 
   STANFORD_DEP_PARTMOD, 
   STANFORD_DEP_DEP, 
   STANFORD_DEP_APPOS, 
   STANFORD_DEP_DET, 
   STANFORD_DEP_DOBJ, 
   STANFORD_DEP_PURPCL, 
   STANFORD_DEP_ABBREV, 
   STANFORD_DEP_POBJ, 
   STANFORD_DEP_IOBJ, 
   STANFORD_DEP_EXPL, 
   STANFORD_DEP_PREDET, 
   STANFORD_DEP_PRECONJ,
   STANFORD_DEP_COUNT 
};

const unsigned long STANFORD_DEP_COUNT_BITS = 6;

/*==============================================================
 *
 * dependency lab
 *
 *==============================================================*/

class CDependencyLabel {

public:

   enum {NONE=0};
   enum {ROOT=1};
   enum {FIRST=1};
   enum {COUNT=STANFORD_DEP_COUNT};
   enum {SIZE=STANFORD_DEP_COUNT_BITS};

protected:

   unsigned long m_code;

public:

   CDependencyLabel() : m_code(NONE) {}
   CDependencyLabel(const unsigned long &code) : m_code(code) { }
   CDependencyLabel(const string &str) { load(str); }
   virtual ~CDependencyLabel() {}

public:

   const unsigned long &hash() const { return m_code; }
   bool operator == (const CDependencyLabel &l) const { return m_code == l.m_code; }
   bool operator != (const CDependencyLabel &l) const { return m_code != l.m_code; }
   bool operator < (const CDependencyLabel &l) const { return m_code < l.m_code; }
   bool operator > (const CDependencyLabel &l) const { return m_code > l.m_code; }
   bool operator <= (const CDependencyLabel &l) const { return m_code <= l.m_code; }
   bool operator >= (const CDependencyLabel &l) const { return m_code >= l.m_code; }

   void load(const string &str) { 
      m_code = STANFORD_DEP_NONE;
      for (int i=FIRST; i<COUNT; ++i) {
         if (STANFORD_DEP_STRINGS[i]==str) {
            m_code = i;
            return;
         }
      }
   }

   void load(const unsigned long &u) { 
      m_code = u;
   }

   const string &str() const { 
      return STANFORD_DEP_STRINGS[ m_code ]; 
   }

   const unsigned long &code() const {
      return m_code;
   }

};

/*==============================================================
 *
 * dependency constraints
 *
 *==============================================================*/

inline bool canAssignLabel(const vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&lab) {
   assert(head==DEPENDENCY_LINK_NO_HEAD||head>=0); // correct head
   assert(dep>=0);
   // if the head word is none, only ROOT
   if (head==DEPENDENCY_LINK_NO_HEAD) {
      if (lab.code()==STANFORD_DEP_ROOT) 
         return true;
      return false;
   }
   // for each case
   const unsigned &head_tag = sent[head].tag.code();
   const unsigned &dep_tag = sent[dep].tag.code();
   assert(head!=DEPENDENCY_LINK_NO_HEAD);
   switch (lab.code()) {
   case STANFORD_DEP_ROOT: // now head is not DEPENDENCY_LINK_NO_HEAD
      return false;
   case STANFORD_DEP_CC:
      if (dep_tag == PENN_TAG_CC)
         return true;
      return false; 
   case STANFORD_DEP_NUMBER:
      if (dep_tag == PENN_TAG_CD)
         return true;
      return false; 
   case STANFORD_DEP_CCOMP:
      if (head_tag == PENN_TAG_NOUN_PLURAL ||
//         head_tag == PENN_TAG_ADVERB ||
//         head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
         head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
         head_tag == PENN_TAG_VERB_PAST ||
         head_tag == PENN_TAG_MD ||
         head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
         head_tag == PENN_TAG_PRP ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_VERB ||
         head_tag == PENN_TAG_ADJECTIVE ||
         head_tag == PENN_TAG_VERB_PRES ||
         head_tag == PENN_TAG_CD ||
         head_tag == PENN_TAG_NOUN_PROPER ||
         head_tag == PENN_TAG_NOUN ||
//         head_tag == PENN_TAG_COMMA ||
//         head_tag == PENN_TAG_TO ||
//         head_tag == PENN_TAG_POS ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
         head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
         head_tag == PENN_TAG_VERB_PROG)
         return true;
      return false; 
   case STANFORD_DEP_POSSESSIVE:
      if ( (head_tag == PENN_TAG_NOUN_PLURAL ||
//         head_tag == PENN_TAG_DT ||
         head_tag == PENN_TAG_PRP ||
         head_tag == PENN_TAG_NOUN_PROPER ||
         head_tag == PENN_TAG_CD ||
//         head_tag == PENN_TAG_ADJECTIVE ||
         head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_NOUN_PROPER_PLURAL) &&
         dep_tag == PENN_TAG_POS )
         return true;
      return false; 
   case STANFORD_DEP_PRT:
      if (head_tag == PENN_TAG_VERB_PROG ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_VERB ||
//         head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_VERB_PRES ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
         head_tag == PENN_TAG_VERB_PAST)
         return true;
      return false; 
   case STANFORD_DEP_NUM:
      if (head_tag == PENN_TAG_NOUN_PLURAL ||
//         head_tag == PENN_TAG_ADVERB ||
//         head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
//         head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
//         head_tag == PENN_TAG_DT ||
         head_tag == PENN_TAG_CD ||
         head_tag == PENN_TAG_ADJECTIVE ||
//         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_NOUN ||
//         head_tag == PENN_TAG_PDT ||
         head_tag == PENN_TAG_DOLLAR)
         return true;
      return false; 
   case STANFORD_DEP_NSUBJPASS:
      if ( head_tag == PENN_TAG_VERB_PAST_PARTICIPATE &&
         (dep_tag == PENN_TAG_NOUN_PLURAL ||
//         dep_tag == PENN_TAG_DT ||
         dep_tag == PENN_TAG_PRP ||
         dep_tag == PENN_TAG_CD ||
//         dep_tag == PENN_TAG_ADJECTIVE ||
         dep_tag == PENN_TAG_DOLLAR ||
         dep_tag == PENN_TAG_WP ||
         dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
         dep_tag == PENN_TAG_NOUN_PROPER ||
         dep_tag == PENN_TAG_NOUN ||
         dep_tag == PENN_TAG_WDT )
//         dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||)
         )
         return true;
      return false; 
   case STANFORD_DEP_CSUBJ: // unsure
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PAST) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_MEASURE: 
      if (
          (head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_DOLLAR)
         )
         return true;
      return false;
   case STANFORD_DEP_CONJ: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_CC ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_WRB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_PERIOD ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_PRP_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_RP ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_MD ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_WRB ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_UH ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_EX ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_SHART ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES)
         )
         return true;
      return false;
   case STANFORD_DEP_AMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
//          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_SYM ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
//          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_ADJECTIVE ||
//          head_tag == PENN_TAG_WP ||
          head_tag == PENN_TAG_FW ||
//          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_NOUN ||
//          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_DOLLAR ||
//          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS) &&
          (dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_SHART ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          dep_tag == PENN_TAG_FW)
         )
         return true;
      return false;
   case STANFORD_DEP_PRED: 
      if (
          (head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_VERB)
         )
         return true;
      return false;
   case STANFORD_DEP_NN: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_NEG: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_CC ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_WRB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          dep_tag == PENN_TAG_ADVERB
         )
         return true;
      return false;
   case STANFORD_DEP_CSUBJPASS: 
      if (
          (head_tag == PENN_TAG_VERB_PAST_PARTICIPATE) &&
          (dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_VERB_PAST)
         )
         return true;
      return false;
   case STANFORD_DEP_MARK: 
      if (
          (head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_VERB_PROG) &&
          dep_tag == PENN_TAG_IN 
         )
         return true;
      return false;
   case STANFORD_DEP_AUXPASS: 
      if (
          (head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_INFMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL) &&
          (dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_REL: 
      if (
          (head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_WDT)
         )
         return true;
      return false;
   case STANFORD_DEP_ADVCL: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_MD ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_AUX: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_MD ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_PREP: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_WDT ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_WP ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_RP ||
          dep_tag == PENN_TAG_FW)
         )
         return true;
      return false;
   case STANFORD_DEP_PARATAXIS: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_PERIOD ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PAST) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_MD ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_POS ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_NSUBJ: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_PRP_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WDT ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_PDT ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_UH ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_POS ||
          dep_tag == PENN_TAG_SYM ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES)
         )
         return true;
      return false;
   case STANFORD_DEP_RCMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PDT ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_MD ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WDT ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES)
         )
         return true;
      return false;
   case STANFORD_DEP_ADVMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_WDT ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_WRB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_WP ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_RP ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WDT ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_WRB ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_UH ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_LS ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_EX ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN)
         )
         return true;
      return false;
   case STANFORD_DEP_PUNCT: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_CC ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_WRB ||
          head_tag == PENN_TAG_R_QUOTE ||
          head_tag == PENN_TAG_R_BRACKET ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_PERIOD ||
          head_tag == PENN_TAG_WP ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_SYM ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_PRP_DOLLAR ||
          head_tag == PENN_TAG_L_QUOTE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_LS) &&
          (dep_tag == PENN_TAG_COMMA ||
          dep_tag == PENN_TAG_L_QUOTE ||
          dep_tag == PENN_TAG_L_BRACKET ||
          dep_tag == PENN_TAG_PERIOD ||
          dep_tag == PENN_TAG_R_QUOTE ||
          dep_tag == PENN_TAG_R_BRACKET ||
          dep_tag == PENN_TAG_COLUM)
         )
         return true;
      return false;
   case STANFORD_DEP_QUANTMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_PDT ||
          head_tag == PENN_TAG_DOLLAR) &&
          (dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_PDT ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE)
         )
         return true;
      return false;
   case STANFORD_DEP_TMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PAST) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE)
         )
         return true;
      return false;
   case STANFORD_DEP_ACOMP: 
      if (
          (head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_ADJECTIVE)
         )
         return true;
      return false;
   case STANFORD_DEP_COMPLM: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DT)
         )
         return true;
      return false;
   case STANFORD_DEP_PCOMP: 
      if (
          (head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_TO) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_VERB_PAST)
         )
         return true;
      return false;
   case STANFORD_DEP_POSS: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_R_QUOTE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_WP_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_POS)
         )
         return true;
      return false;
   case STANFORD_DEP_XCOMP: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_COMMA ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PAST) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_RP ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_VERB_PAST)
         )
         return true;
      return false;
   case STANFORD_DEP_COP: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_PRP_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_VERB_PAST)
         )
         return true;
      return false;
   case STANFORD_DEP_ATTR: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WP)
         )
         return true;
      return false;
   case STANFORD_DEP_PARTMOD: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_VERB_PAST)
         )
         return true;
      return false;
   case STANFORD_DEP_DEP: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_CC ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_RP ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_WDT ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_WRB ||
          head_tag == PENN_TAG_R_QUOTE ||
          head_tag == PENN_TAG_R_BRACKET ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_UH ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_PERIOD ||
          head_tag == PENN_TAG_WP ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS ||
          head_tag == PENN_TAG_SYM ||
          head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_SHART ||
          head_tag == PENN_TAG_L_QUOTE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_RP ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WDT ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_MD ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_WRB ||
          dep_tag == PENN_TAG_R_BRACKET ||
          dep_tag == PENN_TAG_PDT ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_UH ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_LS ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_POS ||
          dep_tag == PENN_TAG_SYM ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_EX ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_SHART ||
          dep_tag == PENN_TAG_L_QUOTE ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES ||
          dep_tag == PENN_TAG_COLUM)
         )
         return true;
      return false;
   case STANFORD_DEP_APPOS: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_VERB_PAST)
         )
         return true;
      return false;
   case STANFORD_DEP_DET: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_POS) &&
          (dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_WDT)
         )
         return true;
      return false;
   case STANFORD_DEP_DOBJ: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WDT ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_WRB ||
          dep_tag == PENN_TAG_R_QUOTE ||
          dep_tag == PENN_TAG_PDT ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_UH ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_POS ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_EX ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PRES)
         )
         return true;
      return false;
   case STANFORD_DEP_PURPCL: 
      if (
          (head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_TO ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB_PROG)
         )
         return true;
      return false;
   case STANFORD_DEP_ABBREV: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL) &&
          (dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN)
         )
         return true;
      return false;
   case STANFORD_DEP_POBJ: 
      if (
          (head_tag == PENN_TAG_FW ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_TO) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_IN ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_WDT ||
          dep_tag == PENN_TAG_VERB_PROG ||
          dep_tag == PENN_TAG_FW ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          dep_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
          dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_UH ||
          dep_tag == PENN_TAG_VERB_PAST ||
          dep_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_WP ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          dep_tag == PENN_TAG_POS ||
          dep_tag == PENN_TAG_ADVERB_COMPARATIVE ||
          dep_tag == PENN_TAG_EX ||
          dep_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          dep_tag == PENN_TAG_PRP_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_NOUN)
         )
         return true;
      return false;
   case STANFORD_DEP_IOBJ: 
      if (
          (head_tag == PENN_TAG_VERB_PROG ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PAST) &&
          (dep_tag == PENN_TAG_NOUN_PLURAL ||
          dep_tag == PENN_TAG_NOUN_PROPER ||
          dep_tag == PENN_TAG_PRP ||
          dep_tag == PENN_TAG_DT ||
          dep_tag == PENN_TAG_CD ||
          dep_tag == PENN_TAG_NOUN ||
          dep_tag == PENN_TAG_VERB ||
          dep_tag == PENN_TAG_ADJECTIVE ||
          dep_tag == PENN_TAG_DOLLAR ||
          dep_tag == PENN_TAG_NOUN_PROPER_PLURAL)
         )
         return true;
      return false;
   case STANFORD_DEP_EXPL: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_MD ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_DOLLAR ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PROG) &&
          dep_tag == PENN_TAG_EX 
         )
         return true;
      return false;
   case STANFORD_DEP_PREDET: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
          head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_NOUN) &&
          dep_tag == PENN_TAG_PDT
         )
         return true;
      return false;
   case STANFORD_DEP_PRECONJ: 
      if (
          (head_tag == PENN_TAG_NOUN_PLURAL ||
          head_tag == PENN_TAG_ADVERB ||
          head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
          head_tag == PENN_TAG_VERB_PAST ||
          head_tag == PENN_TAG_TO ||
          head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
          head_tag == PENN_TAG_PRP ||
          head_tag == PENN_TAG_NOUN_PROPER ||
          head_tag == PENN_TAG_CD ||
          head_tag == PENN_TAG_NOUN ||
          head_tag == PENN_TAG_VERB_PRES ||
          head_tag == PENN_TAG_IN ||
          head_tag == PENN_TAG_ADJECTIVE ||
          head_tag == PENN_TAG_DT ||
          head_tag == PENN_TAG_VERB ||
          head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
          head_tag == PENN_TAG_VERB_PROG) &&
          (dep_tag == PENN_TAG_ADVERB ||
          dep_tag == PENN_TAG_PDT ||
          dep_tag == PENN_TAG_CC ||
          dep_tag == PENN_TAG_DT)
         )
         return true;
      return false;
   default:
      THROW("Invalid label code in assign label: " << lab.code());
   }
}

inline const bool hasLeftHead(const unsigned &tag) {
   switch(tag){
      case PENN_TAG_MD:
//      case PENN_TAG_WRB:
      case PENN_TAG_PDT:
      case PENN_TAG_L_BRACKET:
      case PENN_TAG_WP:
      case PENN_TAG_EX:
      case PENN_TAG_PRP_DOLLAR:
      case PENN_TAG_SHART:
      case PENN_TAG_LS:
      case PENN_TAG_WP_DOLLAR:
         return false;
      default:
         return true;
   }
}

inline const bool hasRightHead(const unsigned &tag) {
   switch(tag){
      case PENN_TAG_RP:
      case PENN_TAG_R_BRACKET:
      case PENN_TAG_POS:
         return false;
      default:
         return true;
   }
}

inline const bool canBeRoot(const unsigned &tag) {
   if (!(tag==PENN_TAG_NOUN_PLURAL || tag==PENN_TAG_MD || tag==PENN_TAG_CC || tag==PENN_TAG_CD || tag==PENN_TAG_RP || tag==PENN_TAG_IN || tag==PENN_TAG_COMMA || tag==PENN_TAG_DOLLAR || tag==PENN_TAG_VERB_PROG || tag==PENN_TAG_FW || tag==PENN_TAG_TO || tag==PENN_TAG_DT || tag==PENN_TAG_VERB || tag==PENN_TAG_WRB || tag==PENN_TAG_VERB_THIRD_SINGLE || tag==PENN_TAG_ADVERB || tag==PENN_TAG_UH || tag==PENN_TAG_VERB_PAST || tag==PENN_TAG_ADJECTIVE_SUPERLATIVE || tag==PENN_TAG_PRP || tag==PENN_TAG_VERB_PAST_PARTICIPATE || tag==PENN_TAG_ADJECTIVE || tag==PENN_TAG_PERIOD || tag==PENN_TAG_WP || tag==PENN_TAG_NOUN_PROPER_PLURAL || tag==PENN_TAG_POS || tag==PENN_TAG_SYM || tag==PENN_TAG_ADVERB_COMPARATIVE || tag==PENN_TAG_ADJECTIVE_COMPARATIVE || tag==PENN_TAG_L_QUOTE || tag==PENN_TAG_NOUN_PROPER || tag==PENN_TAG_NOUN || tag==PENN_TAG_VERB_PRES))
      return false;
   return true;
}

};

#endif
