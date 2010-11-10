// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * stanford.h - the Stanford Dependency style dependency labels *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_RUL_STANFORD
#define _DEPENDENCY_RUL_STANFORD

#include "tags.h"
#include "dependency/label/stanford.h"

namespace english {

/*==============================================================
 *
 * dependency constraints
 *
 *==============================================================*/

inline bool canAssignLabel(const std::vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&lab) {
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
      if ((head_tag == PENN_TAG_NOUN_PLURAL ||
//         head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
         head_tag == PENN_TAG_NOUN_PROPER ||
         head_tag == PENN_TAG_CD ||
         head_tag == PENN_TAG_NOUN ||
//         head_tag == PENN_TAG_ADJECTIVE ||
         head_tag == PENN_TAG_DOLLAR ||
         head_tag == PENN_TAG_NOUN_PROPER_PLURAL) &&
         (dep_tag == PENN_TAG_NOUN_PLURAL ||
         dep_tag == PENN_TAG_ADJECTIVE ||
         dep_tag == PENN_TAG_DT ||
         dep_tag == PENN_TAG_CD ||
         dep_tag == PENN_TAG_NOUN ||
         dep_tag == PENN_TAG_DOLLAR))
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
