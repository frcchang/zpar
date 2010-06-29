import sys
import prolog
import ccgbank_format
import pipe
import getopt

format = ccgbank_format
output = pipe

if __name__ == "__main__":
   hint = "usage: parsetree2BEtag.py [--input prolog/pipe/ccgbank] [--output prolog/pipe/ccgbank] file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:o:", ["output=", "input="])
   format = pipe
   for opt, val in optlist:
      if opt == "--input" or opt == '-i':
         if val == "prolog":
            format = prolog
         elif val == 'ccgbank':
            format = ccgbank_format
         else:
            format = pipe
      elif opt == "--output" or opt=='-o':
         if val == "prolog":
            output = prolog
         elif val == 'ccgbank':
            output = ccgbank_format
         else:
            output = pipe
   file = open(args[0])
   tree = format.LoadTree(file)
   while tree:
      print output.PrintTree(tree)
      tree = format.LoadTree(file)
   file.close()
