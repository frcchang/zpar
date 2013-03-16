#****************************************************************
#
# binarize.py - binarize parsetree.
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

class CBinarizedTreeNode(object):
   slots = ['type',        # 'constituent' / 'token'
            'name',        # ['NP', 'JJ'] 
            'pos',         # only for 'c'; empty for 't' and other types
            'token',       # 'Oxford'
            'left_child',  # left child
            'right_child', # right child
            'head_child'   # lexical head
            'head_leaf'    # head word
            'temporary'    # temporary node?
            ]

   def __init__(self, bTemporary=False):
      self.temporary = bTemporary
      pass

   def __str__(self):
      if self.type == 'constituent':
         sType = self.head_child
         if self.head_child == 's':
            sContent = self.left_child.__str__()
         else:
            sContent = self.left_child.__str__() + " " + self.right_child.__str__()
      elif self.type == 'token':
         if self.pos == '':
            sType = 't'
            sContent = gb2utf(self.token)
         else:
            sType = 'c'
            sContent = self.pos + ' ' + gb2utf(self.token)
      else:
         raise "Type not defined for node"
      if self.temporary:
         sType += "*"
      return "( %s %s %s ) " % (self.name, sType, sContent)

   def prettyprint(self, escape=lambda x:x):
      if self.type == 'constituent':
         sType = self.head_child
         if self.head_child == 's':
            sContent = self.left_child.prettyprint()
         else:
            sContent = self.left_child.prettyprint() + " " + self.right_child.prettyprint()
      elif self.type == 'token':
         sContent = escape(self.token)
      return "( %s %s ) " % (escape(self.name), sContent)

   def load(self, string):
      self.load_list(string.split(), 0)

   def load_list(self, lToken, nIndex):
      assert lToken[nIndex] == "("
      nIndex += 1
      self.name = lToken[nIndex]
      nIndex += 1
      sType = lToken[nIndex]
      nIndex += 1
      self.pos = ""
      assert len(sType) <= 2
      if sType[0] == "l" or sType[0] == "r" or sType[0] == "e":
         self.type = "constituent"
         self.head_child = sType[0]
         self.left_child = CBinarizedTreeNode()
         self.right_child = CBinarizedTreeNode()
         nIndex = self.left_child.load_list(lToken, nIndex)
         nIndex = self.right_child.load_list(lToken, nIndex)
         if sType[0] == 'l':
            self.head_leaf = self.left_child.head_leaf
         else:
            self.head_leaf = self.right_child.head_leaf
      elif sType[0] == "s":
         self.type = "constituent"
         self.head_child = sType[0]
         self.left_child = CBinarizedTreeNode()
         self.right_child = None
         nIndex = self.left_child.load_list(lToken, nIndex)
         self.head_leaf = self.left_child.head_leaf
      else:
         assert sType[0] == "t" or sType[0] == "c"
         self.type = "token"
         if sType[0] == "c":
            self.pos = lToken[nIndex] # self.x?
            nIndex += 1
         self.token = lToken[nIndex]
         nIndex += 1
         while lToken[nIndex] != ")": # space in token maybe
            self.token += " %s"%lToken[nIndex]
            nIndex += 1
         self.head_leaf = self
      if len(sType) == 2:
         assert sType[1] == "*"
         self.temporary = True
      assert lToken[nIndex] == ")"
      return nIndex + 1

   def __setattribute__(self, name, value):
      if name == 'type':
         assert value == 'constituent' or value == 'token'
      if name == 'head_child':
         assert value == 'l' or value == 'r' or value == 's' or value == 'e'
      if name == 'temporary':
         assert value == True or value == False

   def copy(self, node):
      self.type = node.type
      self.name = node.name
      if node.type == 'token':
         self.token = node.token
      else:
         self.left_child = node.left_child

         self.right_child = node.right_child

         self.head_child = node.head_child#######child

         self.temporary = node.temporary

         if 'head_leaf' in dir(node) : self.head_leaf = node.head_leaf

   def tokens(self):
      if self.type == 'constituent':
         if(self.head_child=='s'):
            return self.left_child.tokens()
         else:
            return self.left_child.tokens()+" "+self.right_child.tokens()
      elif self.type == 'token':
         return "%s_%s" % (self.token, self.name)

#================================================================

class CBinarizer(object):

   def __init__(self, sFile, sLogs, bBinarize, bRemoveUnary, sDictionary):
      self.m_dRules = {}
      file = open(sFile)
      line = file.readline()
      while line:
         line = line.strip()
         lPos = line.split(":")
         assert len(lPos) == 2
         sHead = lPos[0].strip()
         self.m_dRules[sHead]=[ru.strip().split() for ru in lPos[1].split(";")] # {NP: [[r, NP, VP], [r]]}
         line = file.readline()
      file.close()
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None
      self.m_bBinarize = bBinarize
      self.m_bRemoveUnary = bRemoveUnary
      self.m_mapDict = {}
      if sDictionary != None:
         file = open(sDictionary)
         for line in file:
            line = line.split()
            self.m_mapDict[line[0]] = line[1]
         file.close()

   def __del__(self):
      if self.log != None:
         self.log.close()

   def escape(self, sText):
      retval = []
      for c in sText:
         retval.append(self.m_mapDict.get(c, c))
      return ''.join(retval)

   def find_head(self, srcnode):
      sLabel = srcnode.name.split('-')[0]
      for lRuleSet in self.m_dRules.get(sLabel, []):
         assert lRuleSet[0] in ("l", "r")
         lTemp = srcnode.children[:]
         if lRuleSet[0] == "r":
            lTemp.reverse()
         bFound = False
         if len(lRuleSet) == 1: # only directn
            nIndex = 0
            headchild = lTemp[nIndex]
            while self.not_empty(headchild) == False:
               nIndex += 1
               headchild = lTemp[nIndex]
            bFound = True
         else:
            for sHead in lRuleSet[1:]:
               for srcchild in lTemp:
                  if srcchild.name.split("-")[0] == sHead and self.not_empty(srcchild):
                     headchild = srcchild
                     bFound = True
                     break
               if bFound:
                  break
         if bFound:
            break
      else:
         if self.log != None:
            print >> self.log, "can't find a rule for " + sLabel + " with " + ", ".join([child_node.name for child_node in srcnode.children])
         nIndex = -1
         headchild = srcnode.children[nIndex]
         while self.not_empty(headchild) == False:
            nIndex = nIndex-1
            headchild = srcnode.children[nIndex]
      assert self.not_empty(headchild)
      return headchild

   def not_empty(self, srcnode):
      if srcnode.name == "-NONE-":
         return False
      if srcnode.type == "token":
         return True
      for srcchild in srcnode.children:
         if self.not_empty(srcchild):
            return True
      return False

   def build_node(self, node, srcnode):
#      if self.m_bRemoveUnary:
#         while srcnode.type == "constituent" and len(srcnode.children) == 1:
#            srcnode = srcnode.children[0]
      if srcnode.name == '-NONE-':
         return False
      node.type = srcnode.type
      node.name = self.escape(srcnode.name.split("-")[0])
      node.start_index = srcnode.start_index
      node.end_index = srcnode.end_index
      if node.type == 'token':
         node.token = self.escape(srcnode.token)
         
      if srcnode.type == "constituent":
         node.children = []
         for srcchildnode in srcnode.children:
            childnode = fidtree.CTreeNode()
            if self.build_node(childnode, srcchildnode):
               node.children.append(childnode)
         if node.children == []:
            return False
         if self.m_bRemoveUnary and len(node.children)==1:
            node.copy(node.children[0])
      return True

   def build_binarized_node(self, node, srcnode):

      # remove all unary reduces
      if self.m_bRemoveUnary:
         while srcnode.type == "constituent" and len(srcnode.children) == 1:
            srcnode = srcnode.children[0]

      if srcnode.name == '-NONE-':
         return False
      srcname = srcnode.name.split("-")[0].split("=")[0]
      node.name = srcname
      node.type = srcnode.type
      if srcnode.type == 'token':
         node.token = srcnode.token
         node.pos = ''   #added by Ding
      else:
         assert srcnode.type == 'constituent'
         # one child node
         tuplechildren = []
         for srcchild in srcnode.children:
            child = CBinarizedTreeNode()
            if self.build_binarized_node(child, srcchild):
               tuplechildren.append((srcchild, child))
         if tuplechildren == []:
            return False
         if len(tuplechildren) == 1:
            if self.m_bRemoveUnary or node.name == tuplechildren[0][1].name:
               node.copy(tuplechildren[0][1])
            else:
               node.left_child = tuplechildren[0][1]
               node.right_child = None
               node.head_child = 's'
            #done
         else:
            headchild = self.find_head(srcnode)
            # two child 
            if len(tuplechildren) == 2:
               node.left_child = tuplechildren[0][1]
               node.right_child = tuplechildren[1][1]
               if headchild == tuplechildren[0][0]:
                  node.head_child = 'l'
               else:
                  if headchild != tuplechildren[1][0]:
                     print headchild, tuplechildren[0][0], tuplechildren[1][0]
                  assert headchild == tuplechildren[1][0]
                  node.head_child = 'r'
               return True
            # more than two child
            else:
               lTemp = tuplechildren[:]
               while len(lTemp) > 2:
                  current = lTemp[0]
                  if current[0] == headchild:
                     break
                  tempnode = CBinarizedTreeNode(True)
                  tempnode.name = srcname
                  tempnode.type = 'constituent'
                  node.left_child = current[1]
                  node.right_child = tempnode
                  node.head_child = 'r'
                  node = tempnode
                  lTemp.pop(0)
               while len(lTemp) > 2:
                  current = lTemp[-1]
                  if current[0] == headchild:
                     break
                  tempnode = CBinarizedTreeNode(True)
                  tempnode.name = srcname
                  tempnode.type = 'constituent'
                  node.left_child = tempnode
                  node.right_child = current[1]
                  node.head_child = 'l'
                  node = tempnode
                  lTemp.pop(-1)
            # now there are exactly two left
            assert len(lTemp) == 2
            node.left_child = lTemp[0][1]
            node.right_child = lTemp[1][1]
            if headchild == lTemp[0][0]:
               node.head_child = 'l'
            else:
               assert headchild == lTemp[1][0]
               node.head_child = 'r'
      return True

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
        # print head
         if self.m_bBinarize:
            outh = CBinarizedTreeNode()
      #      print outh
            self.build_binarized_node(outh, head)
            print outh
         else:
            outh = fidtree.CTreeNode()
            self.build_node(outh, head)
            print outh

#================================================================

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "nul:d:")
   except getopt.GetoptError: 
      print "\nUsage: binarize.py [-nu] [-llogfile] [-ddictionary_file] rule_file input_file > output\n"
      print "-n: not binarize\n"
      print "-u: remove unary nodes\n"
      print "-d: replace with dictionary\n"
      sys.exit(1)
   if len(args) != 2:
      print "\nUsage: binarize.py [-nu] [-llogfile] rule_file input_file > output\n"
      print "-n: not binarize\n"
      print "-u: remove unary nodes\n"
      print "-d: replace with dictionary\n"
      sys.exit(1)
   sLogs = None
   sDictionary = None
   bBinarize = True
   bRemoveUnary = False
   for opt in opts:
      if opt[0] == '-n':
         bBinarize = False
      elif opt[0] == '-l':
         sLogs = opt[1]
      elif opt[0] == '-u':
         bRemoveUnary = True
      elif opt[0] == '-d':
         sDictionary = opt[1]
   rule = CBinarizer(args[0], sLogs, bBinarize, bRemoveUnary, sDictionary)
   file = open(args[1])
   for line in file:
#       print line
       rule.process(line)
   file.close()
