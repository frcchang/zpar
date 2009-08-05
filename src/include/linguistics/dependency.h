/****************************************************************
 *                                                              *
 * dependency.h - the dependency tree                           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_H
#define _DEPENDENCY_H

#include "sentence_template.h"

const int DEPENDENCY_LINK_NO_HEAD = -1;

/*==============================================================
 *
 * CDependencyTreeNode
 *
 *==============================================================*/

class CDependencyTreeNode {

public:
   string word;
   string tag;
   int head;

public:
   CDependencyTreeNode() : word(""), tag(""), head(DEPENDENCY_LINK_NO_HEAD) { }
   CDependencyTreeNode( const string &w, const string &t, const int &h) : word(w), tag(t), head(h) { }
   virtual ~CDependencyTreeNode() {}

public:
   bool operator ==(const CDependencyTreeNode &item) const { 
      return word == item.word && tag == item.tag && head == item.head; 
   }

};

//==============================================================

inline istream & operator >> (istream &is, CDependencyTreeNode &node) {
   (is) >> node.word >> node.tag >> node.head ;
   return is ;
}

inline ostream & operator << (ostream &os, const CDependencyTreeNode &node) {
   os << node.word << "\t" << node.tag << "\t" << node.head ;
   return os ;
}

/*==============================================================
 *
 * CUnlabeledDependencyTree
 *
 * Note that we must define the dependency as sentencetemplate
 * in order to use the standard reading and writing functions
 * defined by the sentence template. Inheritance does not work
 * with template because the generic programming idea does not
 * mix well with the object-oriented programming ideologies. 
 *
 *==============================================================*/

typedef CSentenceTemplate<CDependencyTreeNode> CDependencyTree ;

//==============================================================

#include "dependency_utils.h"

#endif
