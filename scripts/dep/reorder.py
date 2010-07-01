import sys
import depio
import dep

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
         if bPUFound == False and left_child.pos in ['P', 'NT']:
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

def reorder(tree):
   reorderNode(tree.root)
   updateIndex(tree)

#========================================

def posCompare(pos1, pos2):
   order = {'PU':-1, 'NN':10, 'AS':11}
   return cmp(order.get(pos1, 0), order.get(pos2, 0))

def sizeCompare(node1, node2):
   if node1.size > 1.5*node2.size: # significantly smaller
      return -1 # nearer
   if node2.size > 1.5*node1.size:
      return 1
   return 0

def compare(node1, node2):
   retval = posCompare(node1.pos,node2.pos)
   if retval:
      return retval
   retval = sizeCompare(node1, node2)
   return retval

#========================================

if __name__ == '__main__':
   for tree in depio.depread(sys.argv[1]):
      dept = dep.CDep(tree)
      reorder(dept)
      print dept
      #print printOrder(dept)
