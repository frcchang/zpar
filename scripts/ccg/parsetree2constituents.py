import sys
import prolog
import pipe
import getopt

format = pipe
token_index = 2
pos_index = 4

def Iter(node, cons):
   assert node
   if node.end_index - node.start_index > 1:
      cons.append([str(node.start_index), str(node.end_index), node.supercategory])
   if node.left:
      Iter(node.left, cons)
   if node.right: 
      Iter(node.right, cons)

def ToCon(tree):
   cons = []
   Iter(tree.root, cons)
   return cons

if __name__ == "__main__":
   hint = "usage: parsetree2constitu.py [--input prolog/pipe] file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:", ["input="])
   format = pipe
   for opt, val in optlist:
      if opt == "--input":
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
      print ' '.join(['|'.join([token[2],token[4]]) for token in tree.tokens])
      for constituent in ToCon(tree):
         print ' '.join(constituent)
      print
      tree = format.LoadTree(file)
   file.close()
