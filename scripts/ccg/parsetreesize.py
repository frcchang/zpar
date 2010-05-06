import sys
import prolog
import pipe
import getopt

if __name__ == "__main__":
   hint = "usage: parsetreesize.py [--input prolog/pipe] file size"
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
   cutoff = int(args[1])
   tree = format.LoadTree(file)
   while tree:
      if len(tree.tokens) <= cutoff:
         print format.PrintTree(tree)
      tree = format.LoadTree(file)
   file.close()
