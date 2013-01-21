// Copyright (C) SUTD 2013
/****************************************************************
 *                                                              *
 * jointconstituent.cpp - the joint constituent tree            *
 *                                                              *
 * Author: Yue Zhang, Meishan Zhang                             *
 *                                                              *
 * SUTD. 2013.1                                                 *
 *                                                              *
 ****************************************************************/

#include "linguistics/jointconstituent.h"

using namespace TARGET_LANGUAGE;

int CJointTree::readNode(std::istream &is) {
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
      if (s[0] == 'l' || s[0] == 'r' || s[0] == 'x' || s[0] == 'y' || s[0] == 'z' ) {
         right = readNode(is);
         node = newNode();
         // l - head left; r / e - head right
         nodes[node].left_child = left;
         nodes[node].right_child = right;
         nodes[node].label = name;
         nodes[node].type = s[0];
         // e - no token; l / r - has token
         nodes[node].temp = temporary;
         nodes[node].token = (s[0]=='l' || s[0] == 'z' || s[0] == 'x') ? nodes.at(left).token : nodes.at(right).token;
         is >> s;
         assert(s==")");
      }
      else if (s[0] == 's' || s[0] == 't') {
         left = readNode(is);
         node = newNode();
         nodes[node].left_child = left;
         nodes[node].constituent.load(name);
         nodes[node].right_child = -1;
         nodes[node].label = name;
         nodes[node].type = s[0];
         nodes[node].token = nodes.at(left).token;
         nodes[node].temp = temporary;
         assert(temporary==false); // single node can't be binarized temp
         is >> s;
         assert(s==")");
      }
      else {
         ASSERT(s[0]=='b' || s[0]=='i', "A leaf node must be tagged b or i, not "<<s[0]);
         std::string token;
         node = newNode();
         nodes[node].label = name;
         nodes[node].type = s[0];
         is >> token;
         is >> s;
         while (s != ")") {
            token = token + " " + s;
            is >> s;
         }
         int word = newWord(token);
         nodes[node].token = word;
         nodes[node].left_child = -1;
         nodes[node].right_child = -1;
      }
      return node;
}

std::string CJointTree::writeNode(int node) const {
      const CJointTreeNode &nd = nodes[node] ;
      std::string name = nd.label;
      char type = nd.type;
      std::string cont;
      if (type == 'l' || type == 'r' || type == 'x' || type == 'y' || type == 'z') { // [1]node type cons
         if (nd.temp)
            type += "*";
         if (nd.single_child)
            cont = writeNode(nd.left_child);
         else {
            cont = writeNode(nd.left_child) + " " + writeNode(nd.right_child);
         }
      }
      else if (type == 's' || type == 't') {
    	  cont = writeNode(nd.left_child);
      }
      else
      {
    	  cont = chars[nd.token];
      }

      return "( " + name + " " + type + " " + cont + " )";
}

//this output doesn't contain any word strucutre information
std::string CJointTree::writeNodeUnbin(int node) const {
   const CJointTreeNode &nd = nodes[node] ;
   std::string name;
   char type = nd.type;
   std::string cont;
   if (type == 'l' || type == 'r' || type == 's' || type == 't') {
      // do not write node label for temp nodes and NONE nodes (fragmented tree)
      if (nd.is_temporary() ) {
         return writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
      }
      else {
         name = nd.label;
         if (nd.single_child())
            cont = writeNodeUnbin(nd.left_child);
         else {
            cont = writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
         }
         return "(" + name + " " + cont + ")";
      }
   }
   else if (type == 'x' || type == 'y' || type == 'z') {
	   return writeNodeUnbin(nd.left_child) + writeNodeUnbin(nd.right_child);
   }
   else { // [2] token
      cont = chars[nd.token];
      return cont;
   }
}

std::string CJointTree::writeNodeUnbinAll(int node) const {
   const CJointTreeNode &nd = nodes[node] ;
   std::string name;
   char type = nd.type;
   std::string cont;
   if (type == 'l' || type == 'r' || type == 's') {
      // do not write node label for temp nodes and NONE nodes (fragmented tree)
      if (nd.is_temporary() ) {
         return writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
      }
      else {
         name = nd.label;
         if (nd.single_child())
            cont = writeNodeUnbin(nd.left_child);
         else {
            cont = writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
         }
         return "(" + name + " " + cont + ")";
      }
   }
   else if (type == 'x' || type == 'y' || type == 'z' || type == 't') {
	   name = nd.label + "#" + type;
	   if (nd.single_child())
		   cont = writeNodeUnbin(nd.left_child);
	   else {
		   cont = writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
	   }
	   return "(" + name + " " + cont + ")";
   }
   else { // [2] token
	   name = nd.label + "#" + type;
	   cont = chars[nd.token];
	   return "(" + name + " " + cont + ")";
   }
}

bool CJointTree::nodesEqual(const CJointTree &tree, int i, int tree_i) const {
      // compare nodes recursively between this and tree two cfgt
      const CJointTreeNode &nd = nodes[i];
      const CJointTreeNode &tree_nd = nodes[tree_i];
      if (nd.type != tree_nd.type)
         return false;
      if (type == 'l' || type == 'r' || type == 'x' || type == 'y' || type == 'z') {
    	  return (nodesEqual(tree, nd.left_child, tree_nd.left_child) && nodesEqual(tree, nd.right_child, tree_nd.right_child));
      }
      else if (type == 's' || type == 't')
      {
    	  return (nodesEqual(tree, nd.left_child, tree_nd.left_child));
      }
      else
      {
    	  return chars[nd.token] == tree.chars[tree_nd.token] ;
      }
   }

