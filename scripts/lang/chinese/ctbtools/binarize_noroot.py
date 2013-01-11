#****************************************************************
#
# binarize_noroot.py - binarize parsetree.
#
# Author: Meishan Zhang
#
# sutd. 2013.1.9
#
#****************************************************************

import fiditer
import fidtree

import os
import sys

import getopt
import codecs
import binarize


#================================================================

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "nuel:d:")
   except getopt.GetoptError: 
      print "\nUsage: binarize.py [-nue] [-llogfile] [-ddictionary_file] rule_file input_file   output\n"
      print "-n: not binarize\n"
      print "-u: remove unary nodes\n"
      print "-d: replace with dictionary\n"
      sys.exit(1)
   if len(args) != 3:
      print "\nUsage: binarize.py [-nu] [-llogfile] rule_file input_file   output\n"
      print "-n: not binarize\n"
      print "-u: remove unary nodes\n"
      print "-e: keep empty nodes\n"
      print "-d: replace with dictionary\n"
      sys.exit(1)
   sLogs = None
   sDictionary = None
   bBinarize = True
   bRemoveUnary = False
   bEmptyKeep = False
   for opt in opts:
      if opt[0] == '-n':
         bBinarize = False
      elif opt[0] == '-l':
         sLogs = opt[1]
      elif opt[0] == '-u':
         bRemoveUnary = True
      elif opt[0] == '-e':
         bEmptyKeep = True
      elif opt[0] == '-d':
         sDictionary = opt[1]
   rule = binarize.CBinarizer(args[0], sLogs, bBinarize, bRemoveUnary, sDictionary, bEmptyKeep)
   file = codecs.open(args[1],encoding='utf-8')
   wfile = codecs.open(args[2],'w',encoding='utf-8')
   for line in file:
#       print line
       rule.process_noroot(line,wfile)
   file.close()
   wfile.close()
