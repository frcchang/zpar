#****************************************************************
#
# binarize_ctb.py - binarize parsetree.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.11
#
#****************************************************************

import fiditer
import fidtree

import os
import sys

import getopt
import binarize

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "nul:")
   except getopt.GetoptError: 
      print "\nUsage: binarize.py [-nu] [-llogfile] rule_file config_file > output\n"
      print "-n: not binarize\n"
      sys.exit(1)
   if len(args) < 2:
      print "\nUsage: binarize.py [-nu] [-llogfile] rule_file config_file > output\n"
      print "-n: not binarize\n"
      print "-u: remove unary nodes\n"
      sys.exit(1)
   sLogs = None
   bBinarize = True
   bRemoveUnary = False
   for opt in opts:
      if opt[0] == '-n':
         bBinarize = False
      elif opt[0] == '-l':
         sLogs = opt[1]
      elif opt[0] == '-u':
         bRemoveUnary = True
   rule = binarize.CBinarizer(args[0], sLogs, bBinarize, bRemoveUnary)
   cf = config.CConfig(args[1])
   fiditer.sentence_iterator(rule.process, cf.directory, cf.range)
