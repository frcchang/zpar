// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * constituent.h - the constituent tree                         *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _CONSTITUENT_H
#define _CONSTITUENT_H

#include "definitions.h"
#include "utils.h"
#include "cfg.h"

namespace TARGET_LANGUAGE {

/*==============================================================
 *
 * CCFGTreeNode
 *
 *==============================================================*/

class CCFGTreeNode {

public:
   bool is_constituent; // constituent or token
   bool single_child; // single or double
   bool head_left; // double children
   bool temp;
   // fields for constituents
   CConstituentLabel constituent;
//   unsigned long constituent;
   int left_child;
   int right_child;
   // fields for tokens and constituents
   int token;

public:
   CCFGTreeNode() : token(-1), left_child(-1), right_child(-1), is_constituent(true), single_child(true), head_left(false), temp(false), constituent() { 
//      token=-1; 
//      left_child=-1; 
//      right_child=-1; 
//      is_constituent = true;
//      single_child = true;
//      head_left = false;
//      temp = false;
//      constituent=CConstituentLabel::NONE;
   }
   virtual ~CCFGTreeNode() {}

public:
   bool operator ==(const CCFGTreeNode &item) const { 
      throw("Can't compare directly because the contents are flexible indice. Compare trees!");
   }
   
   std::string str() const {
     std::ostringstream os;
     if (is_constituent)
        os << "Constituent(left=" << left_child << ", right=" << right_child << ")";
     else os << "Token";
     
     return os.str();
   }

};

/*==============================================================
 *
 * CCFGTree
 *
 * Note that we must define the constituent as sentencetemplate
 * in order to use the standard reading and writing functions
 * defined by the sentence template. Inheritance does not work
 * with template because the generic programming idea does not
 * mix well with the object-oriented programming ideologies. 
 *
 *==============================================================*/

class CCFGTree {

public:
   std::vector<CCFGTreeNode> nodes;
   CTwoStringVector words;
   int root;

public:
   CCFGTree() : root (-1) {}
   virtual ~CCFGTree() {}

public:
   int newNode() { nodes.push_back(CCFGTreeNode()); return nodes.size()-1; }
   int newWord(const std::string &word, const std::string &pos) { words.push_back(std::make_pair(word, pos)); return(words.size()-1); }

   bool empty() const {return nodes.empty()&&words.empty();}
   void clear() {root=-1;nodes.clear(); words.clear();}
   int readNode(std::istream &is);
   std::string writeNode(int node) const;
   std::string writeNodeUnbin(int node) const;
   bool nodesEqual(const CCFGTree &tree, int i, int tree_i) const ;
   std::string str() const {
      if (root == -1)
         return "";
      return writeNode(root);
   }
   std::string str_unbinarized() const {
      if (root == -1)
         return ""; 
      return writeNodeUnbin(root);
   }
   bool operator == (const CCFGTree &tree) const {
      if (root == -1 || tree.root == -1)
         return root == tree.root;
      return nodesEqual(tree, root, tree.root);
   }

   int parent(const int &node) const {
      for (int i=node+1; i<nodes.size(); ++i) {
         if (nodes[i].left_child==node || nodes[i].right_child==node)
            return i;
      }
      return -1;
   }
}; 

//==============================================================

inline std::istream & operator >> (std::istream &is, CCFGTree &tree) {
   tree.clear();
   std::string line;
//   while(is && line.empty())
//      getline(is, line);
   getline(is, line);

   if(is && !line.empty())
   {
      std::istringstream iss(line);
         tree.root=tree.readNode(iss);
   }
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCFGTree &tree) {
   os << tree.str() << std::endl;
   return os ;
}

//==============================================================

inline void UnparseSentence(const CCFGTree *parsed, CTwoStringVector *tagged) {
   (*tagged) = parsed->words;
}

}

//#include "constituent_utils.h"

#endif
