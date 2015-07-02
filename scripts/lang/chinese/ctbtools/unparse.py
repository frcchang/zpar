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

   def __init__(self, sInput, sLogs, sSep):
      self.path = sInput
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None
      self.sep = sSep

   def __del__(self):
      if self.log != None:
         self.log.close()

   def process(self):
      file = open(self.path)
      for line in file:
         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         print srcnode.tokens(self.sep)
      file.close()


#================================================================

if __name__ == '__main__':
   import sys
   import os
   sys.path.append(os.path.join(os.path.dirname(__file__), '../../../'))
   from tools import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "l:s:")
   except getopt.GetOptError: 
      print "\nUsage: unparse.py [-llogfile] parsed_file > output\n"
      print "s -- pos separator"
      sys.exit(1)
   if len(args) != 1:
      print "\nUsage: unparse.py [-llogfile] parsed_file > output\n"
      print "s -- pos separator"
      sys.exit(1)
   sLogs = None
   sSep = '_'
   for opt in opts:
      if opt[0] == '-l':
         sLogs = opt[1]
      elif opt[0] == '-s':
         sSep = opt[1]
   rule = CUnParse(args[0], sLogs, sSep)
   rule.process()
