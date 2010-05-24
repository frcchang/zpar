import sys
import prolog
import pipe
import getopt
import random

format = pipe
token_index = 2
pos_index = 4

def Iter(node):
   assert node
   if node.left:
      if node.right:
         if node.head_left:
            type = 'l'
         else:
            type = 'r'
      else:
         type = 's'
   else:
      assert not node.right
      type = 'c'
   if type == 'c':
      #retval =  '( ' + node.supercategory + ' s ( ' + node.tree.tokens[node.start_index][pos_index] + ' t ' + node.tree.tokens[node.start_index][token_index] + ' ) )'
      retval =  '( ' + node.supercategory + ' ' + type + ' ' + node.tree.tokens[node.start_index][pos_index] + ' ' + node.tree.tokens[node.start_index][token_index] + ' )'
   elif type in ['l', 'r']:
      retval =  '( ' + node.supercategory + ' ' + type + ' ' + Iter(node.left) + ' ' + Iter(node.right) + ' )'
   else:
      assert type == 's'
      retval =  '( ' + node.supercategory + ' ' + type + ' ' + Iter(node.left) + ' )'
   return retval

def ToCon(tree):
   print Iter(tree.root)

if __name__ == "__main__":
   hint = "usage: ccg2zpar [--i prolog/pipe] file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:n:p:", ["input="])
   format = pipe
   for opt, val in optlist:
      if opt == "-i":
         if val == "prolog":
            format = prolog
            token_index = 2
            pos_index = 3
         else:
            format = pipe
            token_index = 2
            pos_index = 4
   file = open(args[0])
   lastid = 0
   tree = format.LoadTree(file)
   while tree:
      for y in range(tree.id-lastid-1):
         print
      lastid = tree.id
      ToCon(tree)
      tree = format.LoadTree(file)
   file.close()
