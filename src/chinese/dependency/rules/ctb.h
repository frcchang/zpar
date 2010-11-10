// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 *  ctb.h - the penn treebank style dependency rules            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

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
