// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * constituent.cpp - the constituent tree                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.11                        *
 *                                                              *
 ****************************************************************/

#include "linguistics/constituent.h"

using namespace TARGET_LANGUAGE;

int CCFGTree::readNode(std::istream &is) {
      int node;
      std::string s, name;
      is >> s;
      assert(s == "(");
      is >> name;
      is >> s;
      assert (s.size() <= 2);
      int left, right;
      bool temporary;
      temporary = false;
      if (s.size()==2) {
         assert(s[1]=='*');
         temporary = true;
      }
      if (s[0] == 'l' || s[0] == 'r' || s[0] == 'e' ) {
         left = readNode(is);
         right = readNode(is);
         node = newNode();
         nodes[node].is_constituent = true;
         nodes[node].single_child = false;
         // l - head left; r / e - head right
         if (s[0] =='l') nodes[node].head_left = true;
         else nodes[node].head_left = false;
         nodes[node].left_child = left;
         nodes[node].right_child = right;
         // e - NONE node; l / r - labeled node
         if (s[0] == 'e') {
            ASSERT(name==CConstituentLabel(CConstituentLabel::NONE).str(), "An empty node has constituent: " << name);
            nodes[node].constituent.clear();
//            nodes[node].constituent = CConstituentLabel::NONE;
         }
         else {
            nodes[node].constituent.load(name);
//            nodes[node].constituent = CConstituentLabel(name).code();
         }
         // e - no token; l / r - has token
         if (s[0] == 'e') {
            nodes[node].token = -1;
         }
         else {
            nodes[node].token = s[0]=='l' ? nodes.at(left).token : nodes.at(right).token;
            ASSERT(nodes[node].token!=-1, "Cannot find the lexical head for node " << name);
         }
         if (s[0] == 'e') ASSERT(temporary==false, "An empty node was marked as temporary");
         nodes[node].temp = temporary;
         is >> s;
         assert(s==")");
      }
      else if (s[0] == 's') {
         left = readNode(is);
         node = newNode();
         nodes[node].is_constituent = true;
         nodes[node].single_child = true;
         nodes[node].left_child = left;
         nodes[node].constituent.load(name);
//         nodes[node].constituent = CConstituentLabel(name).code();
         nodes[node].right_child = -1;
         nodes[node].token = nodes.at(left).token;
         nodes[node].head_left = false;
         nodes[node].temp = temporary;
         assert(temporary==false); // single node can't be binarized temp
         is >> s;
         assert(s==")");
      }
      else {
         ASSERT(s[0]=='t' || s[0]=='c', "A leaf node must be tagged t or c, not "<<s[0]);
         std::string token;
         node = newNode();
         nodes[node].is_constituent = false;
         nodes[node].single_child = false;
         nodes[node].head_left = false;
         if (s[0]=='c') { nodes[node].constituent.load(name); is >> name; }
//         if (s[0]=='c') { nodes[node].constituent = CConstituentLabel(name).code(); is >> name; }
         is >> token;
         is >> s;
         while (s != ")") {
            token = token + " " + s;
            is >> s;
         } 
         int word = newWord(token, name);
         nodes[node].token = word;
         nodes[node].left_child = -1;
         nodes[node].right_child = -1;
      }
      return node;
}

std::string CCFGTree::writeNode(int node) const {
      const CCFGTreeNode &nd = nodes[node] ;
      std::string name;
      std::string type;
      std::string cont;
      if (nd.is_constituent) { // [1]node type cons
         if (nd.constituent.empty()) {
//         if (nd.constituent==CConstituentLabel::NONE) {
            type = "e";
            ASSERT(nd.temp == false, "Internal error: a NONE node is marked temporary.");
            ASSERT(nd.head_left==false, "Internal error: a NONE node is marked head_left.");
            ASSERT(nd.single_child==false, "Internal error: a NONE node is marked as unary branching.");
         }
         else {
            if (nd.single_child) 
               type = "s";
            else if (nd.head_left)
               type = "l";
            else 
               type = "r";
         }
         name = nd.constituent.str();
//         name = CConstituentLabel(nd.constituent).str();
         if (nd.temp)
            type += "*";
         if (nd.single_child) 
            cont = writeNode(nd.left_child);
         else {
            cont = writeNode(nd.left_child) + " " + writeNode(nd.right_child);
         }
      }
      else { //[2] node.type token
         if (!nd.constituent.empty()) {
//         if (nd.constituent!=CConstituentLabel::NONE) {
            type = "c";
            name = nd.constituent.str();
//            name = CConstituentLabel(nd.constituent).str();
            cont = words[nd.token].second + " " + words[nd.token].first;
         }
         else {
            name = words[nd.token].second;
            type = "t";
            cont = words[nd.token].first;
         }
      }
      return "( " + name + " " + type + " " + cont + " )";
}

std::string CCFGTree::writeNodeUnbin(int node) const {
   const CCFGTreeNode &nd = nodes[node] ;
   std::string name;
   std::string cont;
   if (nd.is_constituent) { // [1] constituent
      // do not write node label for temp nodes and NONE nodes (fragmented tree)
      if (nd.temp || nd.constituent.empty()) {
//      if (nd.temp || nd.constituent==CConstituentLabel::NONE) {
         return writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
      }
      else {
         name = nd.constituent.str();
//         name = CConstituentLabel(nd.constituent).str();
         if (nd.single_child) 
            cont = writeNodeUnbin(nd.left_child);
         else {
            cont = writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
         }
         return "(" + name + " " + cont + ")";
      }
   }
   else { // [2] token
      if (nd.constituent.empty())
//      if (nd.constituent==CConstituentLabel::NONE)
         name = words[nd.token].second;
      else
         name = nd.constituent.str();
//         name = CConstituentLabel(nd.constituent).str();
      cont = words[nd.token].first;
      return "(" + name + " " + cont + ")";
   }
}

bool CCFGTree::nodesEqual(const CCFGTree &tree, int i, int tree_i) const {
      // compare nodes recursively between this and tree two cfgt
      const CCFGTreeNode &nd = nodes[i];
      const CCFGTreeNode &tree_nd = nodes[tree_i];
      if (nd.is_constituent != tree_nd.is_constituent)
         return false;
      if (nd.is_constituent) {
         if (nd.single_child != tree_nd.single_child)
            return false;
         if (nd.single_child) {
            return (nodesEqual(tree, nd.left_child, tree_nd.left_child));
         }
         else {
            if (nd.head_left != tree_nd.head_left)
               return false;
            return (nodesEqual(tree, nd.left_child, tree_nd.left_child) && nodesEqual(tree, nd.right_child, tree_nd.right_child));
         }
      }
      else {
         assert(nd.is_constituent == false);
         return words[nd.token] == tree.words[tree_nd.token];
      }
   }

