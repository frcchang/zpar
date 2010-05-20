#****************************************************************
#
# extractrules.py - extract rules from a binarized-tree.
#
# Author: Yue Zhang
#
#****************************************************************

import binarize
import fidtree

import getopt

class CRuleExtracter(object):

   def __init__(self, sInput, sLogs):
      self.path = sInput
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None

   def __del__(self):
      if self.log != None:
         self.log.close()

   def extract_rules(self, srcnode, retval):
      if srcnode.type == "token":
         return
      else:
         assert srcnode.type == "constituent"
         item = 'REDUCE '
         if srcnode.head_child == 's':
            item += 'UNARY '
            item += srcnode.constituent
            item += ' , '
            item += srcnode.left_child.constituent
            item += ' : 1'
            retval[0].add(item)
         else:
            item += 'BINARY '
            if srcnode.head_child == 'l':
               item += 'LEFT '
            else:
               item += 'RIGHT '
            if srcnode.temporary:
               item += 'TEMP '
            item += node.constituent
            item += ' , '
            item += node.left_child.constituent
            item += ' , '
            item += node.right_child.constituent
            item += ' : 1'
            retval[1].add(item)
   #----------------------------------------------------------------

   def process(self, retval):

      file = open(self.path)
      for line in file:

         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         self.extract_rules(srcnode, retval)
      file.close()

   def prettyprint(self):
      file = open(self.path)
      for line in file:
         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         print srcnode.prettyprint()
      file.close()


#================================================================

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "l:")
   except getopt.GetOptError: 
      print "\nUsage: extractrules.py [-llogfile] binarized_file unary_rules binary_rules\n"
      sys.exit(1)
   if len(args) != 3:
      print "\nUsage: extractrules.py [-llogfile] binarized_file unary_rules binary_rules\n"

      sys.exit(1)
   sLogs = None
   for opt in opts:
      if opt[0] == '-l':
         sLogs = opt[1]
   retval = [set(), set()] # unary, binary
   rule = CRuleExtracter(args[0], sLogs)
   rule.process(retval)
   unary_file = open(args[1], 'w')
   for item in retval[0]:
      unary_file.write(item)
      unary_file.write('\n')
   unary_file.close()
   binary_file = open(args[2], 'w')
   for item in retval[1]:
      binary_file.write(item)
      binary_file.write('\n')
   binaru_file.close()
