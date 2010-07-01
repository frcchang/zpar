import sys
import depio
import dep

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

def reorderNode(node):
   # TODO

def reorder(tree):
   reorderNode(tree.root)
   updateIndex(tree)

if __name__ == '__main__':
   for tree in depio.depread(sys.argv[1]):
      dept = dep.CDep(tree)
      updateIndex(dept)
      print dept
