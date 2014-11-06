// Copyright (C) SUTD 2013
/****************************************************************
 *                                                              *
 * jointconstituent.h - the constituent tree                    *
 *                                                              *
 * Author: Yue Zhang, Meishan Zhang                             *
 *                                                              *
 * SUTD. 2013.11                                                *
 *                                                              *
 ****************************************************************/

#ifndef _JOINTCONSTITUENT_H
#define _JOINTCONSTITUENT_H

#include "definitions.h"
#include "utils.h"
#include "cfg.h"

namespace TARGET_LANGUAGE {

/*==============================================================
 *
 * CJointTreeNode
 *
 *==============================================================*/

class CJointTreeNode {

public:

   bool temp;
   // fields for constituents
   char type;
   std::string label;
//   unsigned long constituent;
   int left_child;
   int right_child;
   // fields for tokens and constituents
   int token;

public:
   CJointTreeNode() : token(-1), left_child(-1), right_child(-1), type('t'), label(""), temp(false) {
//      token=-1;
//      left_child=-1;
//      right_child=-1;
//      is_constituent = true;
//      single_child = true;
//      head_left = false;
//      temp = false;
//      constituent=CConstituentLabel::NONE;
   }
   virtual ~CJointTreeNode() {}

public:
   bool operator ==(const CJointTreeNode &item) const {
      throw("Can't compare directly because the contents are flexible indice. Compare trees!");
   }

   std::string str() const {
     std::ostringstream os;
     os << type << " " << label;
     return os.str();
   }

   bool head_left() const
   {
	   return type == 'l' || type == 'z';
   }

   bool head_right() const
   {
	   return type == 'r' || type == 'y';
   }

   bool head_both() const
   {
	   return type == 'x';
   }

   bool single_child() const
   {
	   return type == 't' || type == 's' || type == 'b' || type == 'i';
   }

   bool is_constituent() const
   {
	   return type == 'l' || type == 'r' || type == 's';
   }

   bool is_leaf() const
   {
	   return type == 't';
   }

   bool is_parital() const
   {
	   return type == 'x' || type == 'y' || type == 'z' || type == 'b' || type == 'i';
   }


   bool is_temporary() const
   {
	   return temp;
   }



};

/*==============================================================
 *
 * CJointTree
 *
 * Note that we must define the constituent as sentencetemplate
 * in order to use the standard reading and writing functions
 * defined by the sentence template. Inheritance does not work
 * with template because the generic programming idea does not
 * mix well with the object-oriented programming ideologies.
 *
 *==============================================================*/

class CJointTree {

public:
   std::vector<CJointTreeNode> nodes;
   CStringVector chars;
   //just for convenience
   CTwoStringVector words;
   CTwoStringVector partwords;
   CTwoStringVector subwords;
   int root;

public:
   CJointTree() : root (-1) {}
   virtual ~CJointTree() {}

public:
   int newNode() { nodes.push_back(CJointTreeNode()); return nodes.size()-1; }
   int newChar(const std::string &word) { chars.push_back(word); return(chars.size()-1); }
   int newWord(const std::string &word, const std::string &pos) { words.push_back(std::make_pair(word, pos)); return(words.size()-1); }
   int newPartWord(const std::string &word, const std::string &type) { partwords.push_back(std::make_pair(word, type)); return(partwords.size()-1); }
   int newSubWord(const std::string &word, const std::string &pos) { subwords.push_back(std::make_pair(word, pos)); return(subwords.size()-1); }

   bool empty() const {return nodes.empty()&&chars.empty();}
   void clear() {root=-1;nodes.clear(); chars.clear(); words.clear(); partwords.clear(); subwords.clear();}
   int readNode(std::istream &is);
   std::string writeNode(int node) const;
   std::string writeNodeUnbin(int node) const;
   std::string writeNodeUnbinAll(int node) const;
   void updateWords(int node);
   void unparse() {
      if ( root == -1 ) { return; }
      words.clear();
      updateWords(root);
   }
   bool nodesEqual(const CJointTree &tree, int i, int tree_i) const ;
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
   std::string str_unbinarizedall() const {
      if (root == -1)
         return "";
       return writeNodeUnbinAll(root);
   }
   bool operator == (const CJointTree &tree) const {
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

inline std::istream & operator >> (std::istream &is, CJointTree &tree) {
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

inline std::ostream & operator << (std::ostream &os, const CJointTree &tree) {
   os << tree.str() << std::endl;
   return os ;
}

//==============================================================

inline void UnparseSentence(const CJointTree *parsed, CStringVector *raw) {
   (*raw) = parsed->chars;
}

inline void UnparseSentence(const CJointTree *parsed, CTwoStringVector *raw) {
   (*raw) = parsed->words;
}

inline void GetSentencePartWords(const CJointTree *parsed, CTwoStringVector *raw) {
   (*raw) = parsed->partwords;
}

inline void GetSentenceSubWords(const CJointTree *parsed, CTwoStringVector *raw) {
   (*raw) = parsed->subwords;
}

}

//#include "constituent_utils.h"

#endif
