#****************************************************************
#
# unbinarize.py - unbinarize parsetree.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.11
#
#****************************************************************

import binarize
import fidtree

import getopt

class CUnBinarizer(object):

   def __init__(self, sInput, sLogs):
      self.path = sInput
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None

   def __del__(self):
      if self.log != None:
         self.log.close()

   def build_node(self, srcnode):
      if srcnode.type == "token":
         node = fidtree.CTreeNode()
         node.name = srcnode.name
         node.type = srcnode.type
         node.token = srcnode.token
         return [node]
      else:
         assert srcnode.type == "constituent"
         if srcnode.temporary:
            lNode = []
            lNode.extend(self.build_node(srcnode.left_child))
            if srcnode.head_child != "s":
               lNode.extend(self.build_node(srcnode.right_child))
            return lNode
         else:
            node = fidtree.CTreeNode()
            node.name = srcnode.name
            node.type = srcnode.type
            node.children = []
            node.children.extend(self.build_node(srcnode.left_child))
            if srcnode.head_child != "s":
               node.children.extend(self.build_node(srcnode.right_child))
            return [node]
   #----------------------------------------------------------------

   def process(self):

      file = open(self.path)
      for line in file:

         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         nodes = self.build_node(srcnode)
         assert len(nodes) == 1
         print nodes[0]
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
      opts, args = getopt.getopt(sys.argv[1:], "nl:")
   except getopt.GetOptError: 
      print "\nUsage: unbinarize.py [-llogfile] [-n] binarized_file > output\n"
      print "-n not binarize; only pretty print"
      sys.exit(1)
   if len(args) != 1:
      print "\nUsage: unbinarize.py [-llogfile] [-n] binarized_file > output\n"
      print "-n not binarize; only pretty print"
      sys.exit(1)
   sLogs = None
   bUnbinarize = True
   for opt in opts:
      if opt[0] == '-l':
         sLogs = opt[1]
      if opt[0] == '-n':
         bUnbinarize = False
   rule = CUnBinarizer(args[0], sLogs)
   if bUnbinarize:
      rule.process()
   else:
      rule.prettyprint()
