#****************************************************************
#
# unparse.py - unparse parsetree.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.11
#
#****************************************************************

import binarize
import fidtree

import getopt

class CUnParse(object):

   def __init__(self, sInput, sLogs):
      self.path = sInput
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None

   def __del__(self):
      if self.log != None:
         self.log.close()

   def process(self):
      file = open(self.path)
      for line in file:
         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         print srcnode.tokens()
      file.close()


#================================================================

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "nl:")
   except getopt.GetOptError: 
      print "\nUsage: unparse.py [-llogfile] parsed_file > output\n"
      sys.exit(1)
   if len(args) != 1:
      print "\nUsage: unparse.py [-llogfile] parsed_file > output\n"
      sys.exit(1)
   sLogs = None
   for opt in opts:
      if opt[0] == '-l':
         sLogs = opt[1]
   rule = CUnParse(args[0], sLogs)
   rule.process()
