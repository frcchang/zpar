#****************************************************************
#
# parsetree.py - python object for parses.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2009.06
#
#****************************************************************

class CTreeNode(object):
   slots = ['action',      # how the node was constructed
            'supercategory', # super category
            'start_index', # the start index in the parse string
            'end_index',   # the end index in the parse string
            'type',        # constituent or token
            'tree',        # the tree
            'left',        # the left child
            'head_left',   # head dep
            'right'        # the right
            ]

   def __init__(self, parent, type):
      self.tree = parent
      self.type = type

   def __setattr__(self, name, value):
      object.__setattr__(self, name, value)
      if name == 'type':
         assert value == 'constituent' or value == 'token'
         if value == 'token':
            self.left = None; self.right = None; self.head_left = False
      if name == 'left' or name == 'right':
         assert type(value) == CTreeNode and self.type == 'constituent' or value == None and self.type == 'token' or value == None and self.type == 'constituent' and name == 'right'
      if name == 'head_left':
         assert value == True or value == False

#================================================================

class CTree(object):

   slots = ['root', 'id', 'tokens']

