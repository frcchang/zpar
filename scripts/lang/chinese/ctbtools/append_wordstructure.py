#****************************************************************
#
# append_wordstructure.py - append word inner strucuture to cfgs.
#
# Author: Meishan Zhang
#
# sutd. 2013.1.9
#
#****************************************************************

import os
import sys

import getopt
import codecs
import binarize

from tools.encoding.gb2utf import *

class CBinarizedCharLeafTreeNode(object):
   slots = ['type',        # 'constituent' / 'token' / 'word inner structure'
            'name',        #  
            'pos',         # 
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
      if self.type == 'constituent' or self.type == 'token' or self.type == 'inner':
         sType = self.head_child
         if self.head_child == 's' or self.head_child == 't':
            sContent = self.left_child.__str__()
         else:
            sContent = self.left_child.__str__() + " " + self.right_child.__str__()
      elif self.type == 'char':
         sType = self.head_child
         sContent = self.token
      else:
         raise "Type not defined for node"
      if self.temporary:
         sType += "*"
      return "( %s %s %s ) " % (self.name, sType, sContent)

   def utf8print(self):
      if self.type == 'constituent' or self.type == 'token' or self.type == 'inner':
         sType = self.head_child
         if self.head_child == 's' or self.head_child == 't':
            sContent = self.left_child.utf8print()
         else:
            sContent = self.left_child.utf8print() + " " + self.right_child.utf8print()
      elif self.type == 'char':
         sType = self.head_child
         sContent = self.token
      else:
         raise "Type not defined for node"
      if self.temporary:
         sType += "*"
      return "( %s %s %s ) " % (self.name, sType, sContent)
   
   def extractcfg(self):
      if self.type == 'constituent':
         sType = self.head_child
         if self.head_child == 's':
            sContent = self.left_child.extractcfg()
         else:
            sContent = self.left_child.extractcfg() + " " + self.right_child.extractcfg()
      elif self.type == 'token':
         sType = self.head_child
         sContent = self.token
      elif self.type == 'inner' or self.type == 'char' :
         raise "Type not defined for node"
      else:
         raise "Type not defined for node"
      if self.temporary:
         sType += "*"
      return "( %s %s %s ) " % (self.name, sType, sContent)
   
   def extractwordpos(self):
      if self.type == 'constituent':
         sType = self.head_child
         if self.head_child == 's':
            return self.left_child.extractwordpos()
         else:
            return self.left_child.extractwordpos() + " " + self.right_child.extractwordpos()
      elif self.type == 'token':
         sType = self.head_child
         return self.token + "_" + self.pos
      elif self.type == 'inner' or self.type == 'char' :
         raise "Type not defined for node"
      else:
         raise "Type not defined for node"
   
   def strberkeley(self):
      if self.type == 'constituent' or self.type == 'token' or self.type == 'inner':
         sType = self.head_child
         if self.head_child == 's' or self.head_child == 't':
            sContent = self.left_child.strberkeley()
         else:
            sContent = self.left_child.strberkeley() + " " +self.right_child.strberkeley()
      elif self.type == 'char':
         sType = self.head_child
         sContent = self.token
      else:
         raise "Type not defined for node"
      if self.temporary:
         sType += "*"
      return "(%s#%s %s)" % (self.name, sType, sContent)
   
     


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
         self.left_child = CBinarizedCharLeafTreeNode()
         self.right_child = CBinarizedCharLeafTreeNode()
         nIndex = self.left_child.load_list(lToken, nIndex)
         nIndex = self.right_child.load_list(lToken, nIndex)
         if sType[0] == 'l':
            self.head_leaf = self.left_child.head_leaf
         else:
            self.head_leaf = self.right_child.head_leaf
      elif sType[0] == "s":
         self.type = "constituent"
         self.head_child = sType[0]
         self.left_child = CBinarizedCharLeafTreeNode()
         self.right_child = None
         nIndex = self.left_child.load_list(lToken, nIndex)
         self.head_leaf = self.left_child.head_leaf
      elif sType[0] == "t" or sType[0] == "c":
         self.type = "token"
         self.head_child = sType[0]
         self.left_child = CBinarizedCharLeafTreeNode()
         self.right_child = None
         nIndex = self.left_child.load_list(lToken, nIndex)
         self.head_leaf = self.left_child.head_leaf
         self.token = self.left_child.token
         self.pos = self.left_child.name
         assert self.name == self.pos
      elif sType[0] == "z" or sType[0] == "y" or sType[0] == "x":
         self.type = "inner"
         self.head_child = sType[0]
         self.left_child = CBinarizedCharLeafTreeNode()
         self.right_child = CBinarizedCharLeafTreeNode()
         nIndex = self.left_child.load_list(lToken, nIndex)
         nIndex = self.right_child.load_list(lToken, nIndex)
         if sType[0] == 'y':
            self.head_leaf = self.right_child.head_leaf
         else:
            self.head_leaf = self.left_child.head_leaf
         self.token = self.left_child.token + self.right_child.token
         assert self.name == self.left_child.name and self.name == self.right_child.name       
      else:
         assert sType[0] == "b" or sType[0] == "i"
         self.type = "char"
         self.head_child = sType[0]
         self.token = lToken[nIndex]
         nIndex += 1
         self.head_leaf = self
      if len(sType) == 2:
         assert sType[1] == "*"
         self.temporary = True
      assert lToken[nIndex] == ")"     
      return nIndex + 1

   def __setattribute__(self, name, value):
      if name == 'type':
         assert value == 'constituent' or value == 'token' or value == 'inner' or value == 'char'
      if name == 'head_child':
         assert value == 'l' or value == 'r' or value == 's' or value == 'e' or value == 'z' or value == 'y' or value == 'x' or value == 'b' or value == 'i' or value == 't' or value == 'c'
      if name == 'temporary':
         assert value == True or value == False

   def copy(self, node):
      self.type = node.type
      self.name = node.name
      self.pos = node.pos
      self.token = node.token
      self.left_child = node.left_child
      self.right_child = node.right_child
      self.head_child = node.head_child#######child
      self.temporary = node.temporary
      self.head_leaf = node.head_leaf


class CWordStrucutre(object):

   def __init__(self, infile, outfile):
      self.m_dWordStructures = {}
      self.m_dRememainWords = {}
      self.m_outfile = codecs.open(outfile,'w',encoding='utf-8')
      if infile != None:
         file = codecs.open(infile,encoding='utf-8')
         for line in file:
            srcnode = CBinarizedCharLeafTreeNode()
            srcnode.load(line)
            wordpos = "[" + srcnode.token + "][" + srcnode.name + "]"
            self.m_dWordStructures[wordpos] = srcnode
         file.close()
         
   def __del__(self):
      if self.m_outfile != None:
         self.m_outfile.close()
   
   def wordbinarize(self, word, pos):
      wordpos = "[" + word + "][" + pos + "]"
      if self.m_dWordStructures.__contains__(wordpos):
         return self.m_dWordStructures[wordpos]
      if self.m_dRememainWords.__contains__(wordpos):
         return self.m_dRememainWords[wordpos]
      
      wordlen = len(word)
      if wordlen == 1:
         srcnode = CBinarizedCharLeafTreeNode()
         srcnode.type = 'char'
         srcnode.name = pos
         srcnode.pos = None
         srcnode.token = word
         srcnode.head_child = 'b'
         srcnode.left_child = None
         srcnode.right_child = None
         srcnode.temporary = False
         srcnode.head_leaf = srcnode
         return  srcnode
      else:            
         srcnode_left = CBinarizedCharLeafTreeNode()
         srcnode_left.type = 'char'
         srcnode_left.name = pos
         srcnode_left.pos = None
         srcnode_left.token = word[0]
         srcnode_left.head_child = 'b'
         srcnode_left.left_child = None
         srcnode_left.right_child = None
         srcnode_left.temporary = False
         srcnode_left.head_leaf = srcnode_left
         start = 1
         outstr = srcnode_left.token + '_' + srcnode_left.name + '#b'
         while start < wordlen:
            srcnode_right = CBinarizedCharLeafTreeNode()
            srcnode_right.type = 'char'
            srcnode_right.name = pos
            srcnode_right.pos = None
            srcnode_right.token = word[start]
            srcnode_right.head_child = 'i'
            srcnode_right.left_child = None
            srcnode_right.right_child = None
            srcnode_right.temporary = False
            srcnode_right.head_leaf = srcnode_right
            
            outstr = outstr + ' ' +srcnode_right.token + '_' + srcnode_right.name + '#b'
            
            srcnode = CBinarizedCharLeafTreeNode()
            srcnode.type = 'inner'
            srcnode.name = pos
            srcnode.pos = None
            srcnode.token = srcnode_left.token + srcnode_right.token
            srcnode.head_child = 'z'
            srcnode.left_child = srcnode_left
            srcnode.right_child = srcnode_right
            srcnode.temporary = False
            srcnode.head_leaf = srcnode_left.head_leaf
            
            start = start+1
            srcnode_left = srcnode
         self.m_dRememainWords[wordpos] = srcnode_left
         self.m_outfile.write(outstr+"\n")
         return  srcnode_left
         
   def process(self, znode):
      if znode == None:
         return None
      elif znode.type == 'constituent':
         srcnode = CBinarizedCharLeafTreeNode()
         srcnode.type = znode.type
         srcnode.name = znode.name
         srcnode.pos = znode.pos
         srcnode.head_child = znode.head_child
         srcnode.left_child = self.process(znode.left_child)
         srcnode.right_child = self.process(znode.right_child)
         srcnode.temporary = znode.temporary
         if 'head_leaf' in dir(znode) : srcnode.head_leaf = znode.head_leaf
         return  srcnode
      elif znode.type == 'token':
         srcnode = CBinarizedCharLeafTreeNode()
         srcnode.type = znode.type
         srcnode.name = znode.name
         srcnode.pos = znode.pos
         srcnode.token = znode.token
         assert znode.pos == ''
         srcnode.head_child = 't'
         wordtree = self.wordbinarize(srcnode.token,srcnode.name)
         srcnode.left_child = self.process(wordtree)           
         srcnode.right_child = None
         srcnode.temporary = znode.temporary
         if 'head_leaf' in dir(wordtree) : srcnode.head_leaf = wordtree.head_leaf
         return  srcnode
      elif znode.type == 'inner':
         srcnode = CBinarizedCharLeafTreeNode()
         srcnode.type = znode.type
         srcnode.name = znode.name
         srcnode.pos = znode.pos
         srcnode.token = znode.token
         srcnode.head_child = znode.head_child
         srcnode.left_child = self.process(znode.left_child)
         srcnode.right_child = self.process(znode.right_child)
         srcnode.temporary = znode.temporary
         if 'head_leaf' in dir(znode) : srcnode.head_leaf = znode.head_leaf
         return  srcnode
      elif znode.type == 'char':
         srcnode = CBinarizedCharLeafTreeNode()
         srcnode.type = znode.type
         srcnode.name = znode.name
         srcnode.pos = znode.pos
         srcnode.token = znode.token
         srcnode.head_child = znode.head_child
         srcnode.left_child = None
         srcnode.right_child = None
         srcnode.temporary = znode.temporary
         if 'head_leaf' in dir(znode) : srcnode.head_leaf = znode.head_leaf
         return  srcnode
      else:
         raise "Type not defined for node"

       
        

if __name__ == '__main__':
   import sys
   import config
   try:
      opts, args = getopt.getopt(sys.argv[1:], "d:i:o:r:")
   except getopt.GetoptError: 
      print "\nUsage: append_wordstructure.py [-iinfile] [-ddictionary_file] [-ooutfile] [-rdictionaryout_file]\n"
      sys.exit(1)
   sInfile = None
   sDictionary = None
   sOutfile = None
   sDictOutFile = None
   for opt in opts:
      if opt[0] == '-d':
         sDictionary = opt[1]
      elif opt[0] == '-i':
         sInfile = opt[1]
      elif opt[0] == '-o':
         sOutfile = opt[1]
      elif opt[0] == '-r':
         sDictOutFile = opt[1]
   wordstut = CWordStrucutre(sDictionary,sDictOutFile)
   
   if sInfile == None or sOutfile == None or sDictOutFile == None:
      print "\nUsage: append_wordstructure.py [-iinfile] [-ddictionary_file] [-ooutfile] [-rdictionaryout_file]\n"
      sys.exit(1)
   
   file = codecs.open(sInfile,encoding='utf-8')
   wfile = codecs.open(sOutfile,'w',encoding='utf-8')
   for line in file:
      znode = binarize.CBinarizedTreeNode();
      znode.load(line)
      newtree = wordstut.process(znode)
      wfile.write(newtree.utf8print()+"\n")
   wfile.close()