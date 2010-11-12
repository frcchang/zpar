import sys
import depio
import dep
import getopt

def printOrderForNode(node):
   retval = []
   for left_child in node.left_children:
      retval.extend(printOrderForNode(left_child))
   retval.append(str(node.extra))
   for right_child in node.right_children:
      retval.extend(printOrderForNode(right_child))
   return retval

def printOrder(tree):
   retlst = printOrderForNode(tree.root)
   retval = ' '.join(retlst)
   return retval

#========================================

def updateIndexForNode(node, index):
   node.extra = node.word_index
   node.word_index = index
   return index+1

def updateIndexForTree(node, index):
   for left_child in node.left_children:
      index = updateIndexForTree(left_child, index)
   index = updateIndexForNode(node, index)
   for right_child in node.right_children:
      index = updateIndexForTree(right_child, index)
   return index

def updateIndex(tree):
   updateIndexForTree(tree.root, 0)

#========================================

def reorderNode(node):
   # recursion
   for left_child in node.left_children:
      reorderNode(left_child)
   for right_child in node.right_children:
      reorderNode(right_child)
   # ver
   if node.pos == 'VV':
      left_children = []
      bPUFound = False
      while node.left_children:
         left_child = node.left_children.pop(-1) # for each left
         if left_child.pos=='PU': # don't move cross PU
            bPUFound = True
         if bPUFound == False and left_child.pos in ['P', 'NT', 'M', 'CD', 'OD']:
            index = 0
            for index in range(len(node.right_children)):
               if compare(left_child, node.right_children[index])==1:
                  break
            else:
               index += 1
            node.right_children.insert(index, left_child)
         else:
            left_children.insert(0, left_child)
      node.left_children = left_children
   # n.
   elif node.pos in ['NN', 'NR']:
      left_children = []
      while node.left_children:
         left_child = node.left_children.pop(-1) # for each left
         if left_child.size > 3 and left_child.pos == 'DEC' or (left_child.pos == 'DEG' and reorderDEG(left_child)):
            node.right_children.insert(0, left_child)
         else:
            left_children.insert(0, left_child)
      node.left_children = left_children
   # dec
   elif ( node.size > 3 and node.pos == 'DEC' or node.pos == 'DEG' and reorderDEG(node) ) or node.pos == 'LC':
      while node.left_children:
         left_child = node.left_children.pop(0)
         node.right_children.insert(0, left_child)

def reorder(tree):
   reorderNode(tree.root)
   updateIndex(tree)

#========================================

def posCompare(pos1, pos2):
   order = {'PU':-1, 'NT':0, 'NN':10, 'AS':11}
   return cmp(order.get(pos1, 0), order.get(pos2, 0))

def sizeCompare(node1, node2):
   if node1.size > 1.5*node2.size: # significantly smaller
      return -1 # nearer
   if node2.size > 1.5*node1.size:
      return 1
   return 0

def compare(node1, node2):
   retval = sizeCompare(node1, node2)
   if retval:
      return retval
   retval = posCompare(node1.pos,node2.pos)
   return retval

def reorderDEG(node):
   assert node.pos == 'DEG'
   for left_child in node.left_children:
      if left_child.pos == 'PN':
         return False
      if left_child.pos == 'JJ':
         return False
   return True

#========================================

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "o:")
   if len(args) < 1:
      print 'reorder [-oi|r] input'
      sys.exit(0)
   sOutput = 'd'
   for opt, val in opts:
      if opt == '-o':
         sOutput = val
   for tree in depio.depread(args[0]):
      dept = dep.CDep(tree)
      reorder(dept)
      if sOutput == 'd':
         print dept
      elif sOutput == 'r':
         print dept.toRaw()
      elif sOutput == 'i':
         print printOrder(dept)
