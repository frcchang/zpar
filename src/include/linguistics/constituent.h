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
   int parent;
   bool is_constituent; // constituent or token
   bool single_child; // single or double
   bool head_left; // double children
   bool temp;
   // fields for constituents
   unsigned long constituent;
   int left_child;
   int right_child;
   // fields for tokens and constituents
   int token;

public:
   CCFGTreeNode() { 
      token=-1; 
      parent=-1; 
      left_child=-1; 
      right_child=-1; 
      is_constituent = true;
      single_child = true;
      head_left = false;
      temp = false;
      constituent=CConstituentLabel::NONE;
   }
   virtual ~CCFGTreeNode() {}

public:
   bool operator ==(const CCFGTreeNode &item) const { 
      throw("Can't compare directly because the contents are flexible indice. Compare trees!");
   }
   
   string str() const {
     ostringstream os;
     if (is_constituent)
        os << "Constituent(parent=" << parent << ", left=" << left_child << ", right=" << right_child << ")";
     else os << "Token(parent=" << parent << ")";
     
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
   vector<CCFGTreeNode> nodes;
   CTwoStringVector words;
   int root;

public:
   CCFGTree() : root (-1) {}
   virtual ~CCFGTree() {}

public:
   int newNode() { nodes.push_back(CCFGTreeNode()); return nodes.size()-1; }
   int newWord(const string &word, const string &pos) { words.push_back(make_pair(word, pos)); return(words.size()-1); }

   bool empty() const {return nodes.empty()&&words.empty();}
   void clear() {root=-1;nodes.clear(); words.clear();}
   int readNode(istream &is);
   string writeNode(int node) const;
   string writeNodeUnbin(int node) const;
   bool nodesEqual(const CCFGTree &tree, int i, int tree_i) const ;
   string str() const {
      if (root == -1)
         return "";
      return writeNode(root);
   }
   string str_unbinarized() const {
      if (root == -1)
         return ""; 
      return writeNodeUnbin(root);
   }
   bool operator == (const CCFGTree &tree) const {
      if (root == -1 || tree.root == -1)
         return root == tree.root;
      return nodesEqual(tree, root, tree.root);
   }
}; 

//==============================================================

inline istream & operator >> (istream &is, CCFGTree &tree) {
   tree.clear();
   string line;
//   while(is && line.empty())
//      getline(is, line);
   getline(is, line);

   if(is && !line.empty())
   {
      istringstream iss(line);
         tree.root=tree.readNode(iss);
   }
   return is ;
}

inline ostream & operator << (ostream &os, const CCFGTree &tree) {
   os << tree.str() << endl;
   return os ;
}

//==============================================================

inline void UnparseSentence(const CCFGTree *parsed, CTwoStringVector *tagged) {
   (*tagged) = parsed->words;
}

}

//#include "constituent_utils.h"

#endif
