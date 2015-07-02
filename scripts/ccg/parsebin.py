#****************************************************************
#
# parsebin.py - binarized parsetree.
#
# Author: Yue Zhang
#
#****************************************************************

from collections import defaultdict

class CBinarizedTreeNode(object):
   __slots__ = ['type',        # 'constituent' / 'token'
                'parent',      # parent
                'name',        # ['NP', 'JJ'] 
                'token',       # index
                'left_child',  # left child
                'right_child', # right child
                'head_child',  # lexical head
                'head_leaf',   # head word
                'leftmost_leaf', # leftmost word
                'rightmost_leaf', # rghmost word
                'temporary',   # temporary node?
                'tree'         # parse tree
                ]

   def __init__(self, tree, parent, bTemporary=False):
      self.tree = tree
      self.parent = parent
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
         assert self.tree.pos
#         if self.tree.pos == []:
#            sType = 't'
#            sContent = self.token
#         else:
#            sType = 'c'
#            sContent = self.tree.pos[self.token] + ' ' + self.tree.words[self.token]
         sType = 'c'
         sContent = self.tree.pos[self.token] + ' ' + self.tree.words[self.token]
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
      assert len(sType) <= 2
      if sType[0] == "l" or sType[0] == "r" or sType[0] == "e":
         self.type = "constituent"
         self.head_child = sType[0]
         self.left_child = CBinarizedTreeNode(self.tree, self)
         self.right_child = CBinarizedTreeNode(self.tree, self)
         nIndex = self.left_child.load_list(lToken, nIndex)
         nIndex = self.right_child.load_list(lToken, nIndex)
         if sType[0] == 'l':
            self.head_leaf = self.left_child.head_leaf
         else:
            self.head_leaf = self.right_child.head_leaf
         self.leftmost_leaf = self.left_child.leftmost_leaf
         self.rightmost_leaf = self.right_child.rightmost_leaf
      elif sType[0] == "s":
         self.type = "constituent"
         self.head_child = sType[0]
         self.left_child = CBinarizedTreeNode(self.tree, self)
         self.right_child = None
         nIndex = self.left_child.load_list(lToken, nIndex)
         self.head_leaf = self.left_child.head_leaf
         self.leftmost_leaf = self.left_child.leftmost_leaf
         self.rightmost_leaf = self.left_child.rightmost_leaf
      else:
#         assert sType[0] == "t" or sType[0] == "c"
         assert sType[0] == "c"
         self.type = "token"
         if sType[0] == "c":
            self.tree.pos.append(lToken[nIndex])
            nIndex += 1
         token = lToken[nIndex]
         nIndex += 1
         while lToken[nIndex] != ")": # space in token maybe
            token += " %s"%lToken[nIndex]
            nIndex += 1
         self.tree.words.append(token)
         # each word corresponds to a pos; if assertion failed it may be because of mixed t and c
         assert len(self.tree.pos) == len(self.tree.words)
         self.token = len(self.tree.words)-1
         self.head_leaf = self
         self.leftmost_leaf = self
         self.rightmost_leaf = self
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

         self.head_leaf = node.head_leaf
         self.leftmost_leaf = node.leftmost_leaf
         self.rightmost_leaf = node.rightmost_leaf

   def tokens(self):
      if self.type == 'constituent':
         if(self.head_child=='s'):
            return self.left_child.tokens()
         else:
            return self.left_child.tokens()+" "+self.right_child.tokens()
      elif self.type == 'token':
         return "%s_%s" % (self.token, self.name)

class CBinarizedTree(object):

   def __init__(self):
      self.root = CBinarizedTreeNode(self, None)
      self.words = []
      self.pos = []

   def load(self, s):
      self.root.load(s)

def fromString(s):
   node = CBinarizedTree()
   node.load(s)
   return node

def fromFile(f):
   path = open(f)
   for line in path:
      yield fromString(line)

#=================================================

def findroots(path):
   roots = set()
   file = open(path)
   for line in file:
      if not line.strip(): continue
      srctree = fromString(line)
      roots.add(srctree.root.name)
   file.close()
   for root in roots:
      print root,

def findcatsforword(word, path):

   def findcatsfornode(word, node, ret):
      if node.type == 'token':
         if node.tree.words[node.token] == word:
            ret.add(node.name)
      else:
         assert node.left_child
         findcatsfornode(word, node.left_child, ret)
         if node.right_child:
            findcatsfornode(word, node.right_child, ret)

   cats = set()
   file = open(path)
   for line in file:
      if not line.strip(): continue
      srctree = fromString(line)
      findcatsfornode(word, srctree.root, cats)
   file.close()
   for cat in cats:
      print cat,

def shiftreduce(path):

   def findleaves(node, retval):
      if node.type == 'token':
         retval.append(node)
      else:
         findleaves(node.left_child, retval)
         if node.right_child:
            findleaves(node.right_child, retval)

   def nextaction(stack, tree):
      if not stack:
         return "shift"
      node = stack[-1]
      if node.parent == None:
         return "done"
      parent = node.parent
      if parent.head_child == 's':
         return 'reduce unary'
      else:
         assert parent.head_child == 'l' or parent.head_child == 'r'
         if node == node.parent.left_child:
            return   'shift'
         return 'reduce binary'

   def takeaction(action, stack, tree, leaves):
      if action == 'shift':
         node = leaves.pop(0)
         stack.append(node)
      elif action == 'reduce unary':
         node = stack.pop(-1)
         stack.append(node.parent)
      elif action == 'reduce binary':
         node1 = stack.pop(-1)
         node2 = stack.pop(-1)
         assert node1.parent == node2.parent
         stack.append(node1.parent)
      elif action == 'done':
         assert not leaves
      else: assert False # invalid action

   file = open(path)
   for line in file:
      tree = fromString(line)
      leaves = []; findleaves(tree.root, leaves)
      print [tree.words[leave.token] for leave in leaves]
      stack = []
      while True:
         action = nextaction(stack, tree)
         print action
         takeaction(action, stack, tree, leaves)
         if action == "done":
            break
      print

def findbasenp(path):

   import re
   npcom=re.compile('NP(\[[a-z]+\])?$')

   def findbasenpfornode(node, ret):
      # find nps from next level.
      baser = False
      if node.type == 'constituent':
         if node.left_child:
            baser |= findbasenpfornode(node.left_child, ret)
         if node.right_child:
            baser |= findbasenpfornode(node.right_child, ret)
      # find current item
      if not baser and npcom.match(node.name):
         ret.append((node.leftmost_leaf.token, node.rightmost_leaf.token, node.head_leaf.token))
         baser=True
      return baser

   file = open(path)
   for line in file:
      if not line.strip(): continue
      nps = []
      srctree = fromString(line)
      findbasenpfornode(srctree.root, nps)
      print ' '.join(['/'.join(word) for word in zip(srctree.words, srctree.pos)])
      print nps
   file.close()

def maxunary(path):

   def maxunaryfornode(node, ret, seq):
      # find nps from next level.
      unary = 0
      cat = ''
      if node.type == 'constituent':
         if node.left_child:
            left, lcat = maxunaryfornode(node.left_child, ret, seq)
         if node.right_child:
            maxunaryfornode(node.right_child, ret, seq)
         if node.head_child == 's':
            unary = 1+left
            cat = node.left_child.name
      if unary > ret[0]:
         ret[0] = unary
         ret[1] = defaultdict(set)
      if unary:
         ret[1][unary].add(str(seq))
#      ret[1][(node.name, node.left_child.name, lcat)].add(str(seq))
      return unary, cat

   file = open(path)
   value = [0, defaultdict(set)]
   seq = 0
   for line in file:
      if not line.strip(): continue
      seq += 1
      srctree = fromString(line)
      maxunaryfornode(srctree.root, value, seq)
#   print value[0], '\n'.join([' '.join(key)+'   :   ['+' '.join(value[1][key])+']' for key in value[1]])
   print 'Max u:', value[0]
   print 'Sentences:', '\n', '\n'.join(['%s: %s' % (k, value[1][k]) for k in value[1]])
   file.close()

#=================================================

if __name__ == '__main__':
   import sys,os
   sys.path.append(os.path.join(os.path.dirname(__file__), '../../cpp1/scripts/ccg'))
   import pipe

   if len(sys.argv) < 2:
      print 'parsebin.py command'
      print 'commands:'
      print '  findroots'
      print '  findcatsforword'
      print '  shiftreduce'
      print '  findbasenp'
      sys.exit(1)

   command = sys.argv[1]
   if command == 'findroots':
      if len(sys.argv) != 3:
         print 'parsebin.py findroots file'
         sys.exit(1)
      findroots(sys.argv[2])
   elif command == 'findcatsforword':
      if len(sys.argv) != 4:
         print 'parsebin.py findcatsforword word file'
         sys.exit(1)
      findcatsforword(sys.argv[2], sys.argv[3])
   elif command == 'shiftreduce':
      if len(sys.argv) != 3:
         print 'parsebin.py shiftreduce file'
         sys.exit(1)
      shiftreduce(sys.argv[2])
   elif command == 'findbasenp':
      if len(sys.argv) != 3:
         print 'parsebin.py findbasenp file'
         sys.exit(1)
      findbasenp(sys.argv[2])
   elif command == 'maxunary':
      if len(sys.argv) != 3:
         print 'parsebin.py maxunary file'
         sys.exit(1)
      maxunary(sys.argv[2])
   else:
      print 'parsebin.py command'
      print 'commands:'
      print '  findroots'
      sys.exit(0)

