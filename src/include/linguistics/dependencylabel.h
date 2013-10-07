// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * dependencylabel.h - the labeled dependency tree              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.07                        *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_LABEL_H
#define _DEPENDENCY_LABEL_H

#include "dependency.h"

/*==============================================================
 *
 * CLabeledDependencyTreeNode
 *
 *==============================================================*/

class CLabeledDependencyTreeNode : public CDependencyTreeNode {

public:
   std::string label;

public:
   CLabeledDependencyTreeNode( ) : CDependencyTreeNode(), label("") { }
   CLabeledDependencyTreeNode( const std::string &w, const std::string &t, const int &h, const std::string &l) : CDependencyTreeNode(w, t, h), label(l) { }
   virtual ~CLabeledDependencyTreeNode() {}

public:
   bool operator ==(const CLabeledDependencyTreeNode &item) const { 
      return static_cast<CDependencyTreeNode>(*this) == static_cast<CDependencyTreeNode>(item) && label == item.label; 
   }

};

//==============================================================

inline std::istream & operator >> (std::istream &is, CLabeledDependencyTreeNode &node) {
   (is) >> static_cast<CDependencyTreeNode&>(node) >> node.label ;
   if (node.label.empty()) THROW("dependency label is unavailable for the input: " << node.word);
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CLabeledDependencyTreeNode &node) {
   os << static_cast<const CDependencyTreeNode&>(node) << "\t" << node.label ;
   return os ;
}

/*==============================================================
 *
 * CExtendedLabeledDependencyTreeNode
 * Same as above, but with feats and lemma information
 *
 *==============================================================*/

class CExtendedLabeledDependencyTreeNode : public CExtendedDependencyTreeNode {

public:
   std::string label;

public:
   CExtendedLabeledDependencyTreeNode( ) : CExtendedDependencyTreeNode(), label("") { }
   CExtendedLabeledDependencyTreeNode( const std::string &w, const std::string &t, const int &h, const std::string &f, const std::string &lem, const std::string &lab) : CExtendedDependencyTreeNode(w, t, h, f, lem), label(lab) { }
   virtual ~CExtendedLabeledDependencyTreeNode() {}

public:
   bool operator ==(const CExtendedLabeledDependencyTreeNode &item) const {
      return static_cast<CExtendedDependencyTreeNode>(*this) == static_cast<CExtendedDependencyTreeNode>(item) && label == item.label;
   }

};

//==============================================================

inline std::istream & operator >> (std::istream &is, CExtendedLabeledDependencyTreeNode &node) {
   (is) >> static_cast<CExtendedDependencyTreeNode&>(node) >> node.label ;
   if (node.label.empty()) THROW("dependency label is unavailable for the input: " << node.word);
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CExtendedLabeledDependencyTreeNode &node) {
   os << static_cast<const CExtendedDependencyTreeNode&>(node) << "\t" << node.label ;
   return os ;
}

/*==============================================================
 *
 * CLabeledDependencyTree
 *
 * Note that we must define the dependency as sentencetemplate
 * in order to use the standard reading and writing functions
 * defined by the sentence template. Inheritance does not work
 * with template because the generic programming idea does not
 * mix well with the object-oriented programming ideologies. 
 *
 *==============================================================*/

typedef CSentenceTemplate<CLabeledDependencyTreeNode> CLabeledDependencyTree ;
typedef CSentenceTemplate<CExtendedLabeledDependencyTreeNode> CExtendedLabeledDependencyTree ;

#endif
