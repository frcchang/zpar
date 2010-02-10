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

int CCFGTree::readNode(istream &is) {
      int node;
      string s, name;
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
      if (s[0] == 'l' || s[0] == 'r') {
         left = readNode(is);
         right = readNode(is);
         node = newNode();
         nodes.at(left).parent = node;
         nodes.at(right).parent = node;
         nodes[node].is_constituent = true;
         nodes[node].single_child = false;
         if (s[0] =='l') nodes[node].head_left = true;
         else nodes[node].head_left = false;
         nodes[node].left_child = left;
         nodes[node].right_child = right;
         nodes[node].constituent = CConstituent(name, true).code();
         nodes[node].token = s[0]=='l' ? nodes.at(left).token : nodes.at(right).token;
         assert(nodes[node].token!=-1);
         nodes[node].temp = temporary;
         is >> s;
         assert(s==")");
      }
      else if (s[0] == 's') {
         left = readNode(is);
         node = newNode();
         nodes.at(left).parent = node;
         nodes[node].is_constituent = true;
         nodes[node].single_child = true;
         nodes[node].left_child = left;
         nodes[node].constituent = CConstituent(name, true).code();
         nodes[node].right_child = -1;
         nodes[node].token = nodes.at(left).token;
         nodes[node].head_left = false;
         nodes[node].temp = temporary;
         assert(temporary==false); // single node can't be binarized temp
         is >> s;
         assert(s==")");
      }
      else {
         assert(s[0]=='t');
         string token;
         node = newNode();
         nodes[node].is_constituent = false;
         nodes[node].single_child = false;
         nodes[node].head_left = false;
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

string CCFGTree::writeNode(int node) const {
      const CCFGTreeNode &nd = nodes[node] ;
      string name;
      string type;
      string cont;
      if (nd.is_constituent) {
         name = CConstituent(nd.constituent).str();
         if (nd.single_child) 
            type = "s";
         else if (nd.head_left)
            type = "l";
         else
            type = "r";
         if (nd.temp)
            type += "*";
         if (nd.single_child) 
            cont = writeNode(nd.left_child);
         else {
            cont = writeNode(nd.left_child) + " " + writeNode(nd.right_child);
         }
      }
      else {
         name = words[nd.token].second;
         type = "t";
         cont = words[nd.token].first;
      }
      return "( " + name + " " + type + " " + cont + " )";
}

string CCFGTree::writeNodeUnbin(int node) const {
   const CCFGTreeNode &nd = nodes[node] ;
   string name;
   string cont;
   if (nd.is_constituent) {
      if (nd.temp) {
         return writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
      }
      else {
         name = CConstituent(nd.constituent).str();
         if (nd.single_child) 
            cont = writeNodeUnbin(nd.left_child);
         else {
            cont = writeNodeUnbin(nd.left_child) + " " + writeNodeUnbin(nd.right_child);
         }
         return "(" + name + " " + cont + ")";
      }
   }
   else {
      name = words[nd.token].second;
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

