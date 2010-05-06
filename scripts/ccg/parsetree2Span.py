import sys
import prolog
import pipe
import getopt

format = pipe
token_index = 2
pos_index = 4

def Iter(node, btags, etags):
   assert node
   span = node.end_index-node.start_index+1
   btags[node.start_index] = max(btags[node.start_index], span)
   etags[node.end_index] = max(etags[node.end_index], span)
   if node.left:
      Iter(node.left, btags, etags)
   if node.right: 
      Iter(node.right, btags, etags)

def Normalize(tg):
   for index in range(len(tg)):
      if tg[index] > 4:
         tg[index] = 0

def ToTag(tree, choice):
   retval = ""
   btags = [1] * len(tree.tokens); etags = [1] * len(tree.tokens)
   Iter(tree.root, btags, etags)
   Normalize(btags)
   Normalize(etags)
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
   hint = "usage: parsetree2BEtag.py [--input prolog/pipe] [--output BE] file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:o:", ["output=", "input="])
   choice = "BE"
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
      elif opt == "--output":
         choice = val
   file = open(args[0])
   lastid = 0
   tree = format.LoadTree(file)
   while tree:
      for y in range(tree.id-lastid-1):
         print
      lastid = tree.id
      print ToTag(tree, choice)
      tree = format.LoadTree(file)
   file.close()
