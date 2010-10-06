/****************************************************************
 *                                                              *
 * generic.h - the dependency rules.                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

inline bool canAssignLabel(const vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&lab) {
   return true;
}

inline const bool hasLeftHead(const unsigned &tag) {
   return true;
}

inline const bool hasRightHead(const unsigned &tag) {
   return true;
}

inline const bool canBeRoot(const unsigned &tag) {
   return true;
}
