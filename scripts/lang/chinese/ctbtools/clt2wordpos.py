#****************************************************************
#
# clt2wordpos.py - extract word_pos from clt.
#
# Author: Meishan Zhang
#
# sutd. 2013.1.11
#
#****************************************************************


import append_wordstructure
import os
import sys

import getopt
import codecs

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "i:o:")
   except getopt.GetoptError: 
      print "\nUsage: clt2wordpos.py [-iinfile] [-ooutfile]\n"
      sys.exit(1)
   sInfile = None
   sOutfile = None
   for opt in opts:
      if opt[0] == '-i':
         sInfile = opt[1]
      elif opt[0] == '-o':
         sOutfile = opt[1]
   
   if sInfile == None or  sOutfile == None:
      print "\nUsage: clt2wordpos.py [-iinfile] [-ooutfile]\n"
      sys.exit(1)
   
   file = codecs.open(sInfile,encoding='utf-8')
   wfile = codecs.open(sOutfile,'w',encoding='utf-8')
   for line in file:
      znode = append_wordstructure.CBinarizedCharLeafTreeNode();
      znode.load(line)
      wfile.write(znode.extractwordpos()+"\n")
   wfile.close()