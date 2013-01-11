#****************************************************************
#
# unbinarizeclt.py - unbinarize clt.
#
# Author: Meishan Zhang
#
# sutd. 2013.1.11
#
#****************************************************************

import os

import append_wordstructure
import fidtree
import codecs

import sys
sys.path.append(os.path.join(os.path.abspath(os.path.dirname(__file__)), '..', '..', '..', 'tools')) 

import getopt


class CCLTUnBinarizer(object):
   
   def __init__(self, sInput):
      self.path = sInput
   
   def __del__(self):
      pass
         
   
   def build_node(self, srcnode):
      if srcnode.type == "char":
         node = fidtree.CTreeNode()
         node.name = srcnode.name + "#" + srcnode.head_child
         node.type = 'token'
         node.token = srcnode.token
         return [node]
      elif srcnode.type == "constituent" or srcnode.type == 'token' or srcnode.type == 'inner': 
         if srcnode.temporary:
            lNode = []
            lNode.extend(self.build_node(srcnode.left_child))
            if srcnode.head_child != "s":
               lNode.extend(self.build_node(srcnode.right_child))
            return lNode
         else:
            node = fidtree.CTreeNode()
            node.name = srcnode.name
            if srcnode.type == 'token' or srcnode.type == 'inner':
               node.name = srcnode.name + "#" + srcnode.head_child
            node.type = "constituent"
            node.children = []
            node.children.extend(self.build_node(srcnode.left_child))
            if srcnode.head_child != "s" and srcnode.head_child != "t":
               node.children.extend(self.build_node(srcnode.right_child))
            return [node]
   #----------------------------------------------------------------

   def process(self):
      file = codecs.open(self.path,encoding='utf-8')
      for line in file:
         if not line.strip(): print; continue
         srcnode = append_wordstructure.CBinarizedCharLeafTreeNode()
         srcnode.load(line)
         nodes = self.build_node(srcnode)
         assert len(nodes) == 1
         yield nodes[0]
      file.close()
     

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "i:o:")
   except getopt.GetoptError: 
      print "\nUsage: unbinarizeclt.py [-iinfile] [-ooutfile]\n"
      sys.exit(1)
   sInfile = None
   sOutfile = None
   for opt in opts:
      if opt[0] == '-i':
         sInfile = opt[1]
      elif opt[0] == '-o':
         sOutfile = opt[1]
   
   if sInfile == None or  sOutfile == None:
      print "\nUsage: unbinarizeclt.py [-iinfile] [-ooutfile]\n"
      sys.exit(1)
   
   rule = CCLTUnBinarizer(sInfile)
   wfile = codecs.open(sOutfile,'w',encoding='utf-8')
   for node in rule.process():
      wfile.write("(ROOT " + node.__str__()+")\n")
   wfile.close() 