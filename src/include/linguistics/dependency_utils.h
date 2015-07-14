// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * dependency_utils.h - the utils for the dependency tree def   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.3                         *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_UTILS_H
#define _DEPENDENCY_UTILS_H

#include "sentence_string.h"

/*==============================================================
 *
 * dependency tree utility functioned
 *
 *==============================================================*/

/*--------------------------------------------------------------
 *
 * IsValidDependencyTree - check well-formed
 *
 *--------------------------------------------------------------*/

template <class CLabeledOrUnlabeledDependencyTree>
inline
bool IsValidDependencyTree(const CLabeledOrUnlabeledDependencyTree &tree) {
   if ( tree.empty() ) return true;
   int nHead = 0;
   int nLoop = 0;
   int j;
   for ( int i=0; i<static_cast<int>(tree.size()); ++i ) {
      if ( tree.at(i).head == DEPENDENCY_LINK_NO_HEAD ) nHead++ ;
      j = i;
      while ( nLoop & (1<<j) == 0 ) {
         nLoop &= (1<<j); // mark to avoid duplicate checking for head
         j = tree.at(i).head;  // move to head
         if (j==DEPENDENCY_LINK_NO_HEAD) break; // head found
         if (j>=static_cast<int>(tree.size())) return false; // out of the boundary of sentence
         if (j==i) return false; // loop found
      }
   }
   if (nHead==1) return true; return false;
}

/*--------------------------------------------------------------
 *
 * IsProjectiveDependencyTree - check projectivity
 *
 *--------------------------------------------------------------*/

template <class CLabeledOrUnlabeledDependencyTree>
inline
bool IsProjectiveDependencyTree(const CLabeledOrUnlabeledDependencyTree &tree) {
   if (!IsValidDependencyTree(tree)) return false;
   for ( int i=0; i<static_cast<int>(tree.size()); ++i ) {
      int mini = std::min(i, tree.at(i).head);
      int maxi = std::max(i, tree.at(i).head);
      for ( int j=mini+1; j<maxi; ++j )
         if (tree.at(j).head<mini||tree.at(j).head>maxi) return false;
   }
   return true;
}

/*---------------------------------------------------------------
 *
 * UnparseSentence - from dependency tree to raw sentence
 *
 *--------------------------------------------------------------*/

template <class CLabeledOrUnlabeledDependencyTree>
inline
void UnparseSentence(const CLabeledOrUnlabeledDependencyTree *parsed, CStringVector *retval) {
   assert(retval != NULL);
   retval->clear();
   if (parsed == NULL)
      return;
   typename CLabeledOrUnlabeledDependencyTree::const_iterator it;
   for (it=parsed->begin(); it!=parsed->end(); ++it)
      retval->push_back(it->word);
}

/*---------------------------------------------------------------
 *
 * UnparseSentence - from dependency tree to tagged sentence
 *
 *--------------------------------------------------------------*/

template <class CLabeledOrUnlabeledDependencyTree>
inline
void UnparseSentence(const CLabeledOrUnlabeledDependencyTree *parsed, CTwoStringVector *retval) {
   assert(retval != NULL);
   retval->clear();
   if (parsed == NULL)
      return;
   typename CLabeledOrUnlabeledDependencyTree::const_iterator it;
   for (it=parsed->begin(); it!=parsed->end(); ++it)
      retval->push_back(std::make_pair(it->word, it->tag));
}

/*---------------------------------------------------------------
 *
 * UnparseAndDesegmentSentence - from dependency tree to raw sent
 *
 *--------------------------------------------------------------*/

template <class CLabeledOrUnlabeledDependencyTree>
inline
void UnparseAndDesegmentSentence(const CLabeledOrUnlabeledDependencyTree *parsed, CStringVector *retval) {
   assert(retval != NULL);
   retval->clear();
   if (parsed == NULL)
      return;
   typename CLabeledOrUnlabeledDependencyTree::const_iterator it;
   for (it=parsed->begin(); it!=parsed->end(); ++it)
      getCharactersFromUTF8String(it->word, retval);
}

#endif
