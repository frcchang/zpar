// Copyright (C) University of Oxford 2010
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
   std::string word;
   std::string tag;
   int head;

public:
   CDependencyTreeNode() : word(""), tag(""), head(DEPENDENCY_LINK_NO_HEAD) { }
   CDependencyTreeNode( const std::string &w, const std::string &t, const int &h) : word(w), tag(t), head(h) { }
   virtual ~CDependencyTreeNode() {}

public:
   bool operator ==(const CDependencyTreeNode &item) const { 
      return word == item.word && tag == item.tag && head == item.head; 
   }

};

//==============================================================

inline std::istream & operator >> (std::istream &is, CDependencyTreeNode &node) {
   (is) >> node.word >> node.tag >> node.head ;
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CDependencyTreeNode &node) {
   os << node.word << "\t" << node.tag << "\t" << node.head ;
   return os ;
}

/*==============================================================
 *
 * CExtendedDependencyTreeNode
 * A node that includes information about CoNLL feats and lemma
 *
 *==============================================================*/

class CExtendedDependencyTreeNode {

public:
   std::string word;
   std::string tag;
   int head;
   std::string feats;
   std::string lemma;

public:
   CExtendedDependencyTreeNode() : word(""), tag(""), head(DEPENDENCY_LINK_NO_HEAD), feats(""), lemma("") { }
   CExtendedDependencyTreeNode( const std::string &w, const std::string &t, const int &h, const std::string &f, const std::string &l ) : word(w), tag(t), head(h), feats(f), lemma(l) { }
   virtual ~CExtendedDependencyTreeNode() {}

public:
   bool operator ==(const CExtendedDependencyTreeNode &item) const {
      return word == item.word && tag == item.tag && head == item.head && feats == item.feats && lemma == item.lemma;
   }

};

//==============================================================

inline std::istream & operator >> (std::istream &is, CExtendedDependencyTreeNode &node) {
   (is) >> node.word >> node.tag >> node.head >> node.feats >> node.lemma;
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CExtendedDependencyTreeNode &node) {
   os << node.word << "\t" << node.tag << "\t" << node.head << node.feats << node.lemma;
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
typedef CSentenceTemplate<CExtendedDependencyTreeNode> CExtendedDependencyTree ;

//==============================================================

#include "dependency_utils.h"

#endif
