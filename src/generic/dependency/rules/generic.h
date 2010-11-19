// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * generic.h - the dependency rules.                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _GENERIC_DEPENDENCY_RULE_H
#define _GENERIC_DEPENDENCY_RULE_H

#ifdef LABELED
inline bool canAssignLabel(const std::vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&lab) {
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
