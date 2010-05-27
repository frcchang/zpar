#****************************************************************
#
# pipe.py - dealing with the ccg bank format for tree
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2009.06
#
#****************************************************************

import parsetree
import sys

#================================================================

def PrintNode(node):
   retval = ""
   retval += "("
   if node.type == "token":
      retval += "<L "
      retval += "*** "
      retval += node.supercategory
      retval += " "
      retval += node.tree.tokens[node.start_index][-1]
      retval += " "
      retval += node.tree.tokens[node.start_index][2]
      retval += ">\n"
      retval += ")"
      retval += "\n"
   else:
      assert node.type == "constituent"
      retval += "<T "
      retval += "*** "
      retval += node.supercategory
      retval += " * "
      head_left = '0' 
      if node.head_left == False: head_left = '1'
      retval += head_left
      retval += " "
      assert node.left 
      ch = 1 
      if node.right: ch = 2
      retval += str(ch)
      retval += ">\n"
      retval += PrintNode(node.left)
      if ch == 2:
         retval += PrintNode(node.right)
      retval += ")"
      retval += "\n"
   return retval

def PrintTree(tree):
   return "###\n%s"  % PrintNode(tree.root)

def PrintBinarizedNode(node):
   retval = ""
   retval += "("
   if node.type == "token":
      retval += "<L "
      retval += "*** "
      retval += node.name
      retval += " "
      retval += node.pos
      retval += " "
      retval += node.token
      retval += ">\n"
      retval += ")"
      retval += "\n"
   else:
      assert node.type == "constituent"
      retval += "<T "
      retval += "*** "
      retval += node.name
      retval += " * "
      head_left = '0' 
      if node.head_child == 'r': head_left = '1'
      retval += head_left
      retval += " "
      assert node.left_child 
      ch = 1 
      if node.right_child: ch = 2
      retval += str(ch)
      retval += ">\n"
      retval += PrintBinarizedNode(node.left_child)
      if ch == 2:
         retval += PrintBinarizedNode(node.right_child)
      retval += ")"
      retval += "\n"
   return retval

def PrintBinarizedTree(tree):
   return "###\n%s" % PrintBinarizedNode(tree)

#================================================================

def LoadTree(file):
   # make new
   tree = parsetree.CTree() # parse tree
   tree.tokens = []
   tree.root = LoadNode(file, tree)
   if tree.root == None:
      return None
   tree.id = 0
   return tree

# loads tree.tokens
def LoadNode(file, tree):
   # pop all that does not start with ccg
   line = file.readline()
   while not line.startswith("(<"):
      if not line: # EOF
         return None 
      line = file.readline()
   lLine = line.split()
   if lLine[0] == "(<T":
      retval = parsetree.CTreeNode(tree, "constituent")
      assert len(lLine) == 6
      count = lLine[5]
      head_left = True; 
      if lLine[4] == '1': head_left=False
      assert count[-1] == ">"
      count = int(count[:-1])
      if count == 1:
         retval.left = LoadNode(file, tree)
         retval.start_index = retval.left.start_index
         retval.end_index = retval.left.end_index
         retval.right = None
      else:
         assert count == 2
         retval.left = LoadNode(file, tree)
         retval.right = LoadNode(file, tree)
         retval.start_index = retval.left.start_index
         retval.end_index = retval.right.end_index
      retval.supercategory = lLine[2]
      retval.head_left = head_left
      # read ending bracket
      line = file.readline()
      assert line.strip()==')'
   else:
      assert lLine[0] == "(<L"
      retval = parsetree.CTreeNode(tree, "token")
      retval.supercat = lLine[2]
      assert len(lLine) == 5
      token = lLine[4]
      assert token[-1] == '>'
      token = token[:-1]
      retval.start_index = len(tree.tokens)
      retval.end_index = len(tree.tokens)
      retval.supercategory = lLine[2]
      pos = lLine[3]
      tree.tokens.append( (None, retval.start_index, token, None, pos) )
      line = file.readline()
      assert line.strip()==')'
   return retval

