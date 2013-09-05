/**
 * Spanish dependency rules for the CoNLL 2006 (CoNLL-X) label set
 *
 * Carlos Gomez-Rodriguez, Yue Zhang
 *
 * No rules at the moment, so we just use the code for the generic parser.
 * Specific rules can be added here later.
 */

#ifndef _ES06_DEPENDENCY_RULE_H
#define _ES06_DEPENDENCY_RULE_H

#ifdef LABELED
inline bool canAssignLabel(const std::vector< CTaggedWord<spanish::CTag,spanish::TAG_SEPARATOR> > &sent, const int &head, const int &dep, const spanish::CDependencyLabel&lab) {
   return true;
}
#endif

inline const bool hasLeftHead(const unsigned &tag) {
   return true;
}

inline const bool hasRightHead(const unsigned &tag) {
   return true;
}

inline const bool canBeRoot(const unsigned &tag) {
   return true;
}

#endif
