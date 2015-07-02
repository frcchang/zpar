#****************************************************************
#
# prolog.py - dealing with the prolog format for tree
#
# Warning: the format prolog does not by default give head information.
# head_left is set to 'r' by default!
# This might not be true according to the rules.
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
   retval += node.action
   retval += "("
   if node.type == "token":
      retval += str(node.tree.id)
      retval += ","
      retval += str(node.start_index+1)
      retval += ","
      retval += node.supercategory
      retval += ")"
   else:
      assert node.type == "constituent"
      retval += node.supercategory
      retval += ","
      retval += PrintNode(node.left)
      if node.right:
         retval += ","
         retval += PrintNode(node.right)
      retval += ")"
   return retval

def PrintToken(tokens):
   retval = ""
   return retval

def PrintTree(tree):
   retval = "ccg("
   retval += str(tree.id)
   retval += ",\n"
   retval += PrintNode(tree.root)
   retval += ").\n\n"
   retval += PrintToken(tree.tokens)
   retval += "\n"
   return retval

#================================================================

# find the first end bracket that does not match any start bracket
def __find_next_bracket__(line, starting_index):
   count = 0
   index = starting_index
   if not line:
      return -1
   while line[index] != ")" or count != 0:
      if line[index] == "(":
         count += 1
      elif line[index] == ")":
         count -= 1
      index += 1
      if index >= len(line):
         return -1
   return index

def __find_next_comma__(line, starting_index, count=1):
   assert count>0
   index = starting_index
   if not line:
      return -1
   skip = 0 # 0 count; 1 one quote
   num = 0
   while index<len(line):
      if line[index]==',':
         if skip==0:
            num+=1
            if num==count:
               return index
      elif line[index] == "'":
         if skip==0:
            skip=1
         else:
            skip=0
      index+=1
   return -1

def LoadTree(file):
   # pop all that does not start with ccg
   line = file.readline()
   while not line.startswith("ccg("):
      if not line: # EOF
         return None 
      line = file.readline()
   # make new
   tree = parsetree.CTree() # parse tree
   comma_index = line.find(",")
   tree.id = int(line[4:comma_index])
   tree.root, scratch = LoadNode(file, tree)
   assert scratch.strip() == ').'
   tree.tokens = LoadToken(file, tree)
   # successful
   return tree

# retval - (node, line)
def LoadNode(file, tree):
   line = file.readline()
   bracket_index = line.find("(")
   action = line[:bracket_index].strip()
   if action == "lf": # token (tree_id, word_id, the_cat)
      tree_comma_index = line.find(",")
      tree_id = int(line[bracket_index+1:tree_comma_index])
      assert tree_id == tree.id
      word_comma_index = line.find(",",tree_comma_index+1)
      word_id = int(line[tree_comma_index+1:word_comma_index])
      end_bracket_index = __find_next_bracket__(line, word_comma_index+1)
      cat = line[word_comma_index+1:end_bracket_index]
      cat = cat.strip("'")
      # retval
      retval = parsetree.CTreeNode(tree, "token")
      retval.start_index = word_id-1
      retval.end_index = word_id-1
      retval.supercategory = cat
      retval.action = action
      return retval, line[end_bracket_index+1:]
   else:
      if action == "lex": # lex ('N', 'NP'
         first_comma_index = __find_next_comma__(line, bracket_index)
         comma_index = line.find(",", first_comma_index+1)
      elif action in ("lp", "rp"): #lp(',','NP','NP\NP',
         first_comma_index = __find_next_comma__(line, bracket_index)
         second_comma_index = line.find(",", first_comma_index+1)
         comma_index = line.find(",", second_comma_index+1)
      elif action == "conj":
         first_comma_index = __find_next_comma__(line, bracket_index)
         second_comma_index = line.find(",", first_comma_index+1)
         comma_index = line.find(",", second_comma_index+1)
      else:
         comma_index = line.find(",")
      cat = line[bracket_index+1:comma_index]
      cat = cat.strip("'")
      assert line[comma_index+1:].strip() ==""
      # retval
      retval = parsetree.CTreeNode(tree, "constituent")
      retval.left, scratch = LoadNode(file, tree)
      retval.start_index = retval.left.start_index
      if scratch.strip()[0]==",":
         retval.right, scratch = LoadNode(file,tree)
         retval.end_index = retval.right.end_index
      else:
         retval.right = None
         retval.end_index = retval.left.end_index
      assert scratch[0] == ")"
      retval.head_left = False # this is because there is not such information!
      retval.supercategory = cat
      retval.action = action
      return retval, scratch[1:]

def LoadToken(file, tree):
   # pop all that does not starts with 
   line = file.readline()
   while not line.startswith("w("):
      assert line # there must be words after nodes
      line = file.readline().strip()
   retval = []
   while line:
      assert line[0]=='w'; assert line[-1] == '.'
      retval.append(eval(line[1:-1]))
      line = file.readline().strip()
   return retval
