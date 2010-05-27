#****************************************************************
#
# unbinarize.py - unbinarize parsetree.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.11
#
#****************************************************************

import os

import binarize
import fidtree

import sys

import getopt

class CUnBinarizer(object):

   def __init__(self, sInput, sLogs, sDictionaryFile):
      self.path = sInput
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None
      self.m_mapEscape = {}
      if sDictionaryFile != None:
         file = open(sDictionaryFile)
         for line in file:
            line = line.split()
            self.m_mapEscape[line[0]] = line[1]
         file.close()

   def __del__(self):
      if self.log != None:
         self.log.close()

   def escape(self, s):
      r = []
      for c in s:
         r.append(self.m_mapEscape.get(c, c))
      return ''.join(r)

   def build_node(self, srcnode):
      if srcnode.type == "token":
         node = fidtree.CTreeNode()
         node.name = self.escape(srcnode.name)
         node.type = srcnode.type
         node.token = self.escape(srcnode.token)
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
            node.name = self.escape(srcnode.name)
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
         if not line.strip(): print; continue
         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         nodes = self.build_node(srcnode)
         assert len(nodes) == 1
         print nodes[0]
      file.close()

   def prettyprint(self):
      file = open(self.path)
      for line in file:
         if not line.strip(): print; continue
         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         print srcnode.prettyprint(self.escape)
      file.close()

   def topipe(self):
      sys.path.append(os.path.join(os.path.dirname(__file__), '../../../ccg'))
      import pipe
      file = open(self.path)
      for line in file:
         if not line.strip(): print; continue
         srcnode = binarize.CBinarizedTreeNode()
         srcnode.load(line)
         print pipe.PrintBinarizedTree(srcnode)
      file.close()

#================================================================

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "o:l:d:")
   except getopt.GetoptError: 
      print "\nUsage: unbinarize.py [-ddictionary] [-llogfile] [-on|p] binarized_file > output\n"
      print "-o output: n not binarize; only pretty print"
      print "           p to ccg pipe"
      print "-d use dictionary to replace"
      sys.exit(1)
   if len(args) != 1:
      print "\nUsage: unbinarize.py [-llogfile] [-on|p] binarized_file > output\n"
      print "-o output: n not binarize; only pretty print"
      print "           p to ccg pipe"
      print "-d use dictionary to replace"
      sys.exit(1)
   sLogs = None
   bUnbinarize = True
   bPipe = False
   sDictionaryFile = None
   for opt in opts:
      if opt[0] == '-l':
         sLogs = opt[1]
      if opt[0] == '-o':
         if opt[1] == 'n':
            bUnbinarize = False
         elif opt[1] == 'p':
            bUnbinarize = False
            bPipe = True
         else:
            print "warning: ingored output form -o"+opt[1]
      if opt[0] == '-d':
         sDictionaryFile = opt[1]
   rule = CUnBinarizer(args[0], sLogs, sDictionaryFile)
   if bUnbinarize:
      rule.process()
   else:
      if bPipe == False:
         rule.prettyprint()
      else:
         rule.topipe()
