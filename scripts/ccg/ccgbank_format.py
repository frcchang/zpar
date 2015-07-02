#****************************************************************
#
# ccgbank_format.py - dealing with the ccg bank format for tree
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2009.06
#
#****************************************************************

import parsetree
import sys
import re

#================================================================

def PrintNode(node):
   retval = ""
   retval += "("
   if node.type == "token":
      retval += "<L "
      retval += node.supercategory
      retval += " "
      retval += node.tree.tokens[node.start_index][-1]
      retval += " "
      retval += node.tree.tokens[node.start_index][-1]
      retval += " "
      retval += node.tree.tokens[node.start_index][2]
      retval += " "
      retval += node.supercategory
      retval += ">)"
   else:
      assert node.type == "constituent"
      retval += "<T "
      retval += node.supercategory
      retval += " "
      head_left = '0' 
      if node.head_left == False: head_left = '1'
      retval += head_left
      retval += " "
      assert node.left 
      ch = 1 
      if node.right: ch = 2
      retval += str(ch)
      retval += "> "
      retval += PrintNode(node.left)
      if ch == 2:
         retval += " "
         retval += PrintNode(node.right)
      retval += ")"
   return retval

def PrintTree(tree):
   return "ID=%d\n%s" % (tree.id, PrintNode(tree.root))

#================================================================

def LoadTree(file):
   # pop all that does not start with ccg
   line = file.readline()
   while not line.startswith("ID="):
      if not line: # EOF
         return None 
      line = file.readline()
   # make new
   tree = parsetree.CTree() # parse tree
   tree.tokens = []
   regex = re.compile("ID=([0-9]+)")
   match = regex.match(line)
   assert(match); tree.id = int(match.group(1))
   line = file.readline()
   tree.root, index = LoadNode(line, 0, tree)
   return tree

# loads tree.tokens
def LoadNode(line, index, tree):
   while line[index] != '(':
      index += 1
   index += 1
   assert(line[index] == '<')
   index += 1
   if line[index] == 'T':
      retval = parsetree.CTreeNode(tree, "constituent")
      end_index = line.find('>', index)
      lLine = line[index+1:end_index].split()
      while len(lLine) < 3:
         end_index = line.find('>', end_index+1)
         lLine = line[index+1:end_index].split()
      assert len(lLine) == 3
      count = lLine[2]
      head_left = True; 
      if lLine[1] == '1': head_left=False
      count = int(count)
      if count == 1:
         retval.left, index = LoadNode(line, end_index+1, tree)
         retval.start_index = retval.left.start_index
         retval.end_index = retval.left.end_index
         retval.right = None
      else:
         assert count == 2
         retval.left, index = LoadNode(line, end_index+1, tree)
         retval.right, index = LoadNode(line, index, tree)
         retval.start_index = retval.left.start_index
         retval.end_index = retval.right.end_index
      retval.supercategory = lLine[0]
      retval.head_left = head_left
      while line[index] != ')': index += 1
   else:
      assert line[index] == "L"
      retval = parsetree.CTreeNode(tree, "token")
      end_index = line.find('>', index)
      lLine = line[index+1:end_index].split()
      while len(lLine) < 5:
         end_index = line.find('>', end_index+1)
         lLine = line[index+1:end_index].split()
      assert len(lLine) == 5
      token = lLine[3]
      retval.start_index = len(tree.tokens)
      retval.end_index = len(tree.tokens)
      retval.supercategory = lLine[0]
      pos = lLine[2]
      tree.tokens.append( (retval.supercategory, retval.start_index, token, None, pos) )
      index = end_index
      while line[index]!=')': index+= 1
   return retval, index+1

