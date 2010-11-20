// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * penn.h - the penn treebank style dependency labels           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.07                        *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_RUL_PENN
#define _DEPENDENCY_RUL_PENN

#include "tags.h"
#include "dependency/label/penn.h"

namespace english {

/*==============================================================
 *
 * dependency constraints
 *
 *==============================================================*/

#ifdef LABELED
inline bool canAssignLabel(const std::vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&lab) {
   assert(head==DEPENDENCY_LINK_NO_HEAD||head>=0); // correct head
   assert(dep>=0);
   // if the head word is none, only ROOT
   if (head==DEPENDENCY_LINK_NO_HEAD) {
      if (lab.code()==PENN_DEP_ROOT) 
         return true;
      return false;
   }
   // for each case
   const unsigned &head_tag = sent[head].tag.code();
   const unsigned &dep_tag = sent[dep].tag.code();
   assert(head!=DEPENDENCY_LINK_NO_HEAD);
   switch (lab.code()) {
   case PENN_DEP_ROOT: // now head is not DEPENDENCY_LINK_NO_HEAD
      return false;
   case PENN_DEP_AMOD: 
      if (head_tag == PENN_TAG_NOUN_PLURAL ||
         head_tag == PENN_TAG_CD ||
         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_DOLLAR ||
//         head_tag == PENN_TAG_FW ||
         head_tag == PENN_TAG_DT ||
         head_tag == PENN_TAG_WRB ||
         head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
         head_tag == PENN_TAG_ADVERB ||
         head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_ADJECTIVE ||
         head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
         head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
         head_tag == PENN_TAG_NOUN)
         return true;
      return false;
   case PENN_DEP_DEP: 
      return true;
   case PENN_DEP_NMOD: 
      if (head_tag == PENN_TAG_NOUN_PLURAL ||
         head_tag == PENN_TAG_CD ||
         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_DOLLAR ||
         head_tag == PENN_TAG_WDT ||
         head_tag == PENN_TAG_FW ||
         head_tag == PENN_TAG_DT ||
//         head_tag == PENN_TAG_VERB ||
         head_tag == PENN_TAG_WRB ||
//         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
//         head_tag == PENN_TAG_ADVERB_SUPERLATIVE ||
         head_tag == PENN_TAG_ADVERB ||
         head_tag == PENN_TAG_UH ||
         head_tag == PENN_TAG_VERB_PROG ||
         head_tag == PENN_TAG_ADJECTIVE_SUPERLATIVE ||
         head_tag == PENN_TAG_PRP ||
         head_tag == PENN_TAG_ADJECTIVE ||
         head_tag == PENN_TAG_WP ||
         head_tag == PENN_TAG_NOUN_PROPER_PLURAL ||
         head_tag == PENN_TAG_POS ||
//         head_tag == PENN_TAG_ADVERB_COMPARATIVE ||
         head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
         head_tag == PENN_TAG_NOUN_PROPER ||
         head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_WP_DOLLAR)
         return true;
      return false;
   case PENN_DEP_OBJ: 
      if (
//         head_tag == PENN_TAG_NOUN_PLURAL ||
         head_tag == PENN_TAG_VERB_PAST ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_VERB ||
//        head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_VERB_PRES ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
         head_tag == PENN_TAG_VERB_PROG)
         return true;
      return false;
   case PENN_DEP_P: 
      if (dep_tag == PENN_TAG_R_QUOTE ||
         dep_tag == PENN_TAG_SHART ||
         dep_tag == PENN_TAG_L_QUOTE ||
         dep_tag == PENN_TAG_L_BRACKET ||
         dep_tag == PENN_TAG_PERIOD ||
         dep_tag == PENN_TAG_COMMA ||
         dep_tag == PENN_TAG_R_BRACKET ||
         dep_tag == PENN_TAG_DOLLAR ||
         dep_tag == PENN_TAG_COLUM)
         return true;
      return false;
   case PENN_DEP_PMOD: 
      if (head_tag == PENN_TAG_ADVERB ||
//         head_tag == PENN_TAG_SYM ||
         head_tag == PENN_TAG_VERB_PROG ||
         head_tag == PENN_TAG_TO ||
//         head_tag == PENN_TAG_DT ||
         head_tag == PENN_TAG_CC ||
//         head_tag == PENN_TAG_PRP_DOLLAR ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
//         head_tag == PENN_TAG_ADJECTIVE ||
         head_tag == PENN_TAG_IN ||
 //        head_tag == PENN_TAG_RP ||
         head_tag == PENN_TAG_FW)
         return true;
      return false;
   case PENN_DEP_PRD: 
      if (
//         head_tag == PENN_TAG_NOUN_PLURAL ||
//         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_VERB_PROG ||
         head_tag == PENN_TAG_VERB ||
//         head_tag == PENN_TAG_WRB ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
         head_tag == PENN_TAG_VERB_PAST ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
//         head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_VERB_PRES)
         return true;
      return false;
   case PENN_DEP_SBAR: 
      if (head_tag == PENN_TAG_DT ||
//         head_tag == PENN_TAG_ADJECTIVE ||
//         head_tag == PENN_TAG_VERB_PRES ||
         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_WRB ||
         head_tag == PENN_TAG_WP_DOLLAR ||
         head_tag == PENN_TAG_WP ||
         head_tag == PENN_TAG_WDT)
         return true;
      return false;
   case PENN_DEP_SUB: 
      if (
//         head_tag == PENN_TAG_NOUN_PLURAL ||
         head_tag == PENN_TAG_MD ||
         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_VERB_PROG ||
//         head_tag == PENN_TAG_FW ||
         head_tag == PENN_TAG_VERB ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
         head_tag == PENN_TAG_VERB_PAST ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_ADJECTIVE ||
//         head_tag == PENN_TAG_ADJECTIVE_COMPARATIVE ||
//         head_tag == PENN_TAG_NOUN_PROPER ||
         head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_VERB_PRES)
         return true;
      return false;
   case PENN_DEP_VC: 
      if (head_tag == PENN_TAG_VERB_PAST ||
         head_tag == PENN_TAG_MD ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_VERB ||
         head_tag == PENN_TAG_VERB_PRES ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
         head_tag == PENN_TAG_VERB_PROG)
         return true;
      return false;
   case PENN_DEP_VMOD:
      if (head_tag == PENN_TAG_NOUN_PLURAL ||
         head_tag == PENN_TAG_MD ||
         head_tag == PENN_TAG_IN ||
         head_tag == PENN_TAG_VERB_PROG ||
//         head_tag == PENN_TAG_FW ||
         head_tag == PENN_TAG_VERB ||
         head_tag == PENN_TAG_VERB_THIRD_SINGLE ||
//         head_tag == PENN_TAG_ADVERB ||
         head_tag == PENN_TAG_VERB_PAST ||
         head_tag == PENN_TAG_VERB_PAST_PARTICIPATE ||
         head_tag == PENN_TAG_ADJECTIVE ||
//         head_tag == PENN_TAG_NOUN_PROPER ||
         head_tag == PENN_TAG_NOUN ||
         head_tag == PENN_TAG_VERB_PRES)
         return true;
      return false;
   default:
      THROW("Invalid label code in assign label: " << lab.code());
   }
}
#endif

inline bool hasLeftHead(const unsigned &tag) {
   switch(tag){
      case PENN_TAG_PRP_DOLLAR:
      case PENN_TAG_SHART:
      case PENN_TAG_POS:
      case PENN_TAG_L_BRACKET:
      case PENN_TAG_LS:
      case PENN_TAG_EX:
         return false;
      default:
         return true;
   }
}

inline bool hasRightHead(const unsigned &tag) {
   switch(tag){
      case PENN_TAG_WDT:
      case PENN_TAG_RP:
      case PENN_TAG_PERIOD:
      case PENN_TAG_WP_DOLLAR:
      case PENN_TAG_R_BRACKET:
         return false;
      default:
         return true;
   }
}

inline bool canBeRoot(const unsigned &tag) {
   if (!(tag==PENN_TAG_VERB_PROG || tag==PENN_TAG_VERB_PAST || tag==PENN_TAG_VERB_PAST_PARTICIPATE || tag==PENN_TAG_VERB_PRES || tag==PENN_TAG_WDT || tag==PENN_TAG_ADJECTIVE || tag==PENN_TAG_WP || tag==PENN_TAG_VERB_THIRD_SINGLE || tag==PENN_TAG_DT || tag==PENN_TAG_DOLLAR || tag==PENN_TAG_NOUN || tag==PENN_TAG_FW || tag==PENN_TAG_POS || tag==PENN_TAG_TO || tag==PENN_TAG_ADVERB || tag==PENN_TAG_NOUN_PLURAL || tag==PENN_TAG_NOUN_PROPER || tag==PENN_TAG_VERB || tag==PENN_TAG_WRB || tag==PENN_TAG_CC || tag==PENN_TAG_ADVERB_COMPARATIVE || tag==PENN_TAG_CD || tag==PENN_TAG_IN || tag==PENN_TAG_WP_DOLLAR || tag==PENN_TAG_MD || tag==PENN_TAG_NOUN_PROPER_PLURAL || tag==PENN_TAG_ADJECTIVE_COMPARATIVE || tag==PENN_TAG_SYM || tag==PENN_TAG_UH))
      return false;
   return true;
}

};

#endif
