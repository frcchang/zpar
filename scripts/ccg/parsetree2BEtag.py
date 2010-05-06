import sys
import prolog
import pipe
import ccgbank_format
import getopt

format = pipe
token_index = 2
pos_index = 4

def Iter(node, btags, etags):
   assert node
   if node.end_index - node.start_index < 1:
      return
   btags[node.start_index] = 1
   etags[node.end_index] = 1
   assert node.left != None
   Iter(node.left, btags, etags)
   if node.right: 
      Iter(node.right, btags, etags)

def ToTag(tree, choice):
   retval = ""
   btags = [0] * len(tree.tokens); etags = [0] * len(tree.tokens)
   Iter(tree.root, btags, etags)
   index = 0
   for word in tree.tokens:
      if index > 0: 
         retval += " "
      retval += word[token_index]
      if choice != None:
         retval += "|"
         retval += word[pos_index]
      if 'B' in choice :
         retval += "|"
         retval += str(btags[index])
      if 'E' in choice :
         retval += "|"
         retval += str(etags[index])
      index += 1
   return retval

if __name__ == "__main__":
   hint = "usage: [--input prolog/pipe/ccgbank] [--output BE] parsetree2BEtag.py file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:o:", ["output=", "input="])
   choice = 'BE'
   format = pipe
   for opt, val in optlist:
      if opt == "--input":
         if val == "prolog":
            format = prolog
            token_index = 2
            pos_index = 3
         elif val == "ccgbank":
            token_index = 2
            pos_index = 4
            format = ccgbank_format
         else:
            format = pipe
            token_index = 2
            pos_index = 4
      elif opt == "--output":
         choice = val
   file = open(args[0])
   lastid = 0 # yes
   tree = format.LoadTree(file)
   while tree:
      for y in range(tree.id-lastid-1):
         print
      lastid = tree.id
      print ToTag(tree, choice)
      tree = format.LoadTree(file)
   file.close()
