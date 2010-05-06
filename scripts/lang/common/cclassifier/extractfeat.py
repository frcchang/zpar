import sys
import getopt
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))

import constituentreader
import tools.integerization

def extractOneFeat(sent, constituent, integerizer):
   retval = []
   

def extractFeat(path, integerizer):
   for sent, canstituents in constituentreader.read(path):
      for constituent in constituents:
         yield extractOneFeat(sent, constituent, integerizer)

if __name__ == "__main__":
   opts, args = getopt.getopt(sys.argv[1:], "b:", [])
   sIntegerizerPath = ''
   for opt in opts:
      if opt[0] == "-b":
         sIntegerizerPath = opt[1]

   if len(args) < 1:
      print "usage: extractfeatures.py [-b] input >output"
      sys.exit(1)

   if sIntegerizerPath:
      integerizer = tools.integerization.CIntegerization()
   else:
      integerizer = None

   extractFeat(sys.argv[1], integerizer)

   if integerizer:
      integerizer.write(sIntegerizerPath)
