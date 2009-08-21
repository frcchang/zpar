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

//==============================================================

const string PENN_DEP_STRINGS[] = {
   "-NONE-",
   "ROOT", 
   "AMOD",
   "DEP", 
   "NMOD", 
   "OBJ", 
   "P",
   "PMOD", 
   "PRD",
   "SBAR",
   "SUB",
   "VC",
   "VMOD"
};

enum PENN_DEP_LABELS {
   PENN_DEP_NONE=0,
   PENN_DEP_ROOT, 
   PENN_DEP_AMOD, 
   PENN_DEP_DEP, 
   PENN_DEP_NMOD, 
   PENN_DEP_OBJ, 
   PENN_DEP_P, 
   PENN_DEP_PMOD, 
   PENN_DEP_PRD, 
   PENN_DEP_SBAR, 
   PENN_DEP_SUB, 
   PENN_DEP_VC, 
   PENN_DEP_VMOD, 
   PENN_DEP_COUNT 
};

const unsigned PENN_DEP_START = 1;
const unsigned PENN_DEP_COUNT_BITS = 4;

/*==============================================================
 *
 * dependency lab
 *
 *==============================================================*/

class CDependencyLabel {

protected:

   unsigned m_code;

public:

   CDependencyLabel(const unsigned &code) { m_code = code; }
   CDependencyLabel(const string &str) { load(str); }
   virtual ~CDependencyLabel() {}

public:

   void load(const string &str) { 
      m_code = PENN_DEP_NONE;
      for (int i=PENN_DEP_START; i<PENN_DEP_COUNT; i++) {
         if (PENN_DEP_STRINGS[i]==str) {
            m_code = i;
            return;
         }
      }
   }

   string str() { 
      return PENN_DEP_STRINGS[ m_code ]; 
   }

   const unsigned &code() const {
      return m_code;
   }

};

/*==============================================================
 *
 * CLabeledDependencyTreeNode
 *
 *==============================================================*/

class CLabeledDependencyTreeNode : public CDependencyTreeNode {

public:
   string label;

public:
   CLabeledDependencyTreeNode( ) : CDependencyTreeNode(), label(CDependencyLabel(PENN_DEP_NONE).str()) { }
   CLabeledDependencyTreeNode( const string &w, const string &t, const int &h, const string &l) : CDependencyTreeNode(w, t, h), label(l) { }
   virtual ~CLabeledDependencyTreeNode() {}

public:
   bool operator ==(const CLabeledDependencyTreeNode &item) const { 
      return static_cast<CDependencyTreeNode>(*this) == static_cast<CDependencyTreeNode>(item) && label == item.label; 
   }

};

//==============================================================

inline istream & operator >> (istream &is, CLabeledDependencyTreeNode &node) {
   (is) >> static_cast<CDependencyTreeNode&>(node) >> node.label ;
   if (node.label.empty()) THROW("dependency label is unavailable in the input file");
   return is ;
}

inline ostream & operator << (ostream &os, const CLabeledDependencyTreeNode &node) {
   os << static_cast<const CDependencyTreeNode&>(node) << "\t" << node.label ;
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

#endif
