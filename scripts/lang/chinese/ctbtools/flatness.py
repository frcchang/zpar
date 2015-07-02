#****************************************************************
#
# flatness.py - flatness parsetree.
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

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))

from tools.encoding.gb2utf import *

#================================================================

class CFlatness(object):

   def count_flatness(self, srcnode):
      if srcnode.type == 'token':
         return (0,0) # we don't count leafs
      else:
         assert(srcnode.type == "constituent")
         nc = 1
         subc = len(srcnode.children)
      if srcnode.type == "constituent":
         for srcchildnode in srcnode.children:
            x,y=self.count_flatness(srcchildnode)
            nc+=x
            subc+=y
         return (nc,subc)

   def process(self, sSentence):
      # don't process empty sentences
      if sSentence.strip() == "":
         return
      # find the cfg node
      head = fidtree.parse_object(sSentence)
      if type(head) == list:
         lHead = head
      else:
         lHead = [head]                         
      # output the dep node
      for head in lHead:
         outh=self.count_flatness(head)
         print float(outh[1])/float(outh[0])

   
#================================================================

if __name__ == '__main__':
   import sys
   import config
   rule = CFlatness()
   file = open(sys.argv[1])
   for line in file:
#       print line
       rule.process(line)
   file.close()
