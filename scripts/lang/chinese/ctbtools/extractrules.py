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

def adddict(d, i):
   if not i in d:
      d[i] = 1
   else:
      d[i] += 1

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
#            if srcnode.left_child.type != 'token':
            item += 'UNARY '
            item += srcnode.name
            item += ' , '
            item += srcnode.left_child.name
            adddict(retval[0], item)
            self.extract_rules(srcnode.left_child, retval)
         else:
            item += 'BINARY '
            if srcnode.head_child == 'l':
               item += 'LEFT '
            else:
               assert srcnode.head_child == 'r'
               item += 'RIGHT '
            if srcnode.temporary:
               item += 'TEMP '
            item += srcnode.name
            item += ' , '
            item += srcnode.left_child.name
            item += ' , '
            item += srcnode.right_child.name
            adddict(retval[1], item)
            self.extract_rules(srcnode.left_child, retval)
            self.extract_rules(srcnode.right_child, retval)
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
   retval = [{}, {}] # unary, binary
   rule = CRuleExtracter(args[0], sLogs)
   rule.process(retval)
   unary_file = open(args[1], 'w')
   for item in retval[0]:
      unary_file.write(item + ' : ' + str(retval[0][item]))
      unary_file.write('\n')
   unary_file.close()
   binary_file = open(args[2], 'w')
   for item in retval[1]:
      binary_file.write(item + ' : ' + str(retval[1][item]))
      binary_file.write('\n')
   binary_file.close()
