#****************************************************************
#
# parsecfg.py - python object for parses.
#
# Author: Yue Zhang
#
#****************************************************************

class CTreeNode(object):
   slots = ['type',        # 'constituent' / 'token'
            'name',        # ['NP', 'OBJ'] 
            'token',       # 'Oxford'
            'children',    # list of child nodes
            'start_index', # the start index in the parse string
            'end_index',   # the end index in the parse string
            ]

   def __init__(self):
      pass

   def __str__(self):
      if self.type == 'constituent':
         sContent = " ".join([child.__str__() for child in self.children])
      elif self.type == 'token':
         sContent = self.token
      else:
         raise "Type not defined for node"
      return "(%s %s)" % (self.name, sContent)

   def __setattribute__(self, name, value):
      if name == 'type':
         assert value == 'constituent' or value == 'token'

   def copy(self, node):
      self.type = node.type
      self.name = node.name
      if self.type == 'token':
         self.token = node.token
      else:
         self.children = node.children
         self.start_index = node.start_index
         self.end_index = node.end_index

#================================================================

def __next_index(sSentence, nStartIndex):
   nIndex = nStartIndex 
   while nIndex<len(sSentence):
      if not sSentence[nIndex].isspace(): return nIndex
      nIndex += 1
   else:
      return None

def __find_name(sSentence, nStartIndex):
   lToken = []
   nIndex = nStartIndex
   while nIndex<len(sSentence) and not sSentence[nIndex].isspace():
      lToken.append(sSentence[nIndex])
      nIndex = nIndex+1
   if lToken == []:
      raise "No token found after a bracket:\n" + sSentence 
   return "".join(lToken).strip(), nIndex

def __find_token(sSentence, nStartIndex):
   lToken = []
   nIndex = nStartIndex
   nLastIndex = nIndex
   while nIndex<len(sSentence):
      if sSentence[nIndex] == ')':
         break
      nIndex = __next_index(sSentence, nIndex+1)
      lToken.append(sSentence[nLastIndex:nIndex])
      nLastIndex = nIndex
   if lToken == []:
      raise "No token found after a bracket:\n" + sSentence 
   return "".join(lToken).strip(), nIndex

def __find_node(sSentence, nIndex):
   nStart = nIndex
   nIndex = __next_index(sSentence, nIndex)
   if nIndex == None:
      raise "Found empty string while expecting some income: " + sSentence
   if sSentence[nIndex] != "(": raise "( not found from sentence: %s from index: %d" % (sSentence, nIndex)
   # the next case is the out bracket in "( (NP ...) ... )" 
   if sSentence[nIndex+1].isspace() or sSentence[nIndex+1]=="(":
      node = __find_node(sSentence, nIndex+1)
      nIndex = node.end_index
      nIndex = __next_index(sSentence, nIndex+1)
      assert sSentence[nIndex] == "(" or sSentence[nIndex] == ")"
      if sSentence[nIndex] == ")":
         node.start_index = nStart
         node.end_index = nIndex
         return node
      elif sSentence[nIndex]== "(":
         lNodes = [node]
         while sSentence[nIndex] == "(":
            node = __find_node(sSentence, nIndex)
            lNodes.append(node)
            nIndex = __next_index(sSentence, node.end_index+1)
         assert sSentence[nIndex] == ")"
         return lNodes
   # the next case is the out bracket in "(NP ...)" or "(NN car)"
   node = CTreeNode()
   node.start_index = nStart
   sNode, nIndex = __find_name(sSentence, nIndex+1)
   node.name = sNode
   nIndex = __next_index(sSentence, nIndex)
   if sSentence[nIndex] == "(":
      node.type = "constituent"
      lChildren = []
      while sSentence[nIndex] == "(":
         child = __find_node(sSentence, nIndex)
         lChildren.append(child)
         nIndex = child.end_index
         nIndex = __next_index(sSentence, nIndex+1)
      node.children = lChildren
      assert sSentence[nIndex] == ")"
      node.end_index = nIndex
      return node
   else:
      node.type = "token"
      sToken, nIndex = __find_token(sSentence, nIndex)
      node.token = sToken
      assert sSentence[nIndex]==")"
      node.end_index = nIndex
      return node

#================================================================

def fromString(sSentence):
   if sSentence.strip() == "":
      return "Empty sentence"
   return __find_node(sSentence, 0)

#================================================================

if __name__ == '__main__':
   import fiditer
   import config
   cf = config.CConfig("fid.config")
   def printx(x):
      print fromString(x)
   fiditer.sentence_iterator( printx, cf.directory, cf.range )
