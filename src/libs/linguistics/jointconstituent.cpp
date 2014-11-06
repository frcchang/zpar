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
      static std::string temp;
      static std::string type;
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
      	int start_char_pos = chars.size();
      	left = readNode(is);
         right = readNode(is);
         node = newNode();
         // l - head left; r / e - head right
         nodes[node].left_child = left;
         nodes[node].right_child = right;
         nodes[node].label = name;
         nodes[node].type = s[0];
         // l / r - has token
         nodes[node].temp = temporary;
         int end_char_pos = chars.size();

         if (s[0] == 'x' || s[0] == 'y' || s[0] == 'z')
			{
         	type = "";
         	type += s[0];
				temp = "";
				for (int idx=start_char_pos; idx<end_char_pos; ++idx) // append the corresponding characters
					temp += chars.at(idx);
				newPartWord(temp, type);
				newSubWord(temp, name);
			}

         nodes[node].token = (s[0]=='l' || s[0] == 'z' || s[0] == 'x') ? nodes.at(left).token : nodes.at(right).token;
         is >> s;
         assert(s==")");
      }
      else if (s[0] == 's' || s[0] == 't') {
      	int start_char_pos = chars.size();
         left = readNode(is);
         node = newNode();
         nodes[node].left_child = left;
         nodes[node].right_child = -1;
         nodes[node].label = name;
         nodes[node].type = s[0];
         nodes[node].token = nodes.at(left).token;
         nodes[node].temp = temporary;
         int end_char_pos = chars.size();

         if (s[0] == 't')
         {
         	temp = "";
				for (int idx=start_char_pos; idx<end_char_pos; ++idx) // append the corresponding characters
					temp += chars.at(idx);
				newWord(temp, name);
         }
         assert(temporary==false); // single node can't be binarized temp
         is >> s;
         assert(s==")");
      }
      else {
         //ASSERT(s[0]=='b' || s[0]=='i', "A leaf node must be tagged b or i, not "<<s[0]);
      	if(s[0] !='b' && s[0] !='i')
      	{
      		ASSERT(s[0]=='b' || s[0]=='i', "A leaf node must be tagged b or i, not "<<s[0]);
      	}
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
         int word = newChar(token);
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
      std::string stype = "";
      stype += type;

      std::string cont;
      if (type == 'l' || type == 'r' || type == 'x' || type == 'y' || type == 'z') { // [1]node type cons
         if (nd.temp)
        	 stype += "*";
         	 cont = writeNode(nd.left_child) + " " + writeNode(nd.right_child);
      }
      else if (type == 's' || type == 't') {
    	  cont = writeNode(nd.left_child);
      }
      else
      {
    	  cont = chars[nd.token];
      }

      return "( " + name + " " + stype + " " + cont + " )";
}
void CJointTree::updateWords(int node) {

   const CJointTreeNode &nd = nodes[node] ;
   char type = nd.type;
   if (type == 'l' || type == 'r' || type == 's' || type == 't') {
      if ( type == 't' ) {
         std::string pos_tag = nd.label;
         words.push_back(std::make_pair("", pos_tag));
      }
      // do not write node label for temp nodes and NONE nodes (fragmented tree)
      if (nd.is_temporary() ) {
         updateWords(nd.left_child);
         updateWords(nd.right_child);
      }
      else {
         if (nd.single_child()) {
            updateWords(nd.left_child);
         } else {
            updateWords(nd.left_child);
            updateWords(nd.right_child);
         }
      }
   }
   else if (type == 'x' || type == 'y' || type == 'z') {
      updateWords(nd.left_child);
      updateWords(nd.right_child);
   }
   else { // [2] token
      std::string & word = words.back().first;
      word += chars[nd.token];
   }
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
         return writeNodeUnbinAll(nd.left_child) + " " + writeNodeUnbinAll(nd.right_child);
      }
      else {
         name = nd.label;
         if (nd.single_child())
            cont = writeNodeUnbinAll(nd.left_child);
         else {
            cont = writeNodeUnbinAll(nd.left_child) + " " + writeNodeUnbinAll(nd.right_child);
         }
         return "(" + name + " " + cont + ")";
      }
   }
   else if (type == 'x' || type == 'y' || type == 'z' || type == 't') {
	   name = nd.label + "#" + type;
	   if (nd.single_child())
		   cont = writeNodeUnbinAll(nd.left_child);
	   else {
		   cont = writeNodeUnbinAll(nd.left_child) + " " + writeNodeUnbinAll(nd.right_child);
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
      if (nd.label != tree_nd.label)
              return false;

      char type = nd.type;
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

