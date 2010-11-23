import sys
import getopt
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../dep"))
import depio
import dep

#========================================

def readAlign(path):
   retval = {}
   n=0
   file = gzip.open(path)
   for line in file:
      if line.startswith('SENT:'):
#         number = int(line[6:-1])
#         assert number == n # make sure of sentence index
         if n>0:
            yield retval
            retval = {}
         n += 1
      else:
         assert line.startswith('S ')
         target, source = map(int, line[2:-1].split())
         if not source in retval:
            retval[source] = []
         retval[source].append(target)
   yield retval
   file.close()

#========================================

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

def reorderVV(node, align, model):
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

def reorderNN(node, align, model):
   left_children = []
   while node.left_children:
      left_child = node.left_children.pop(-1) # for each left
      if left_child.pos == 'DEC' or (left_child.pos == 'DEG' and deg(left_child)):
         reorderDEG(left_child)
         node.right_children.insert(0, left_child)
      else:
         left_children.insert(0, left_child)
   node.left_children = left_children

def reorderDEG(node, align, model):
   while node.left_children:
      left_child = node.left_children.pop(0)
      node.right_children.insert(0, left_child)

def reorderLC(node, align, model):
   while node.left_children:
      left_child = node.left_children.pop(0)
      node.right_children.insert(0, left_child)

def reorderNode(node, align, model):
   # recursion
   for left_child in node.left_children:
      reorderNode(left_child)
   for right_child in node.right_children:
      reorderNode(right_child)
   # ver
   if node.pos == 'VV':
      reorderVV(node, align, model)
   # n.
   elif node.pos in ['NN', 'NR']:
      reorderNN(node, align, model)
   # lc
   elif node.pos == 'LC':
      reorderLC(node, align, model)

def reorder(tree, align, model):
   reorderNode(tree.root, align, model)
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

def deg(node):
   assert node.pos == 'DEG'
   for left_child in node.left_children:
      if left_child.pos == 'JJ':
         return False
   return True

#========================================

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "o:i:")
   if len(args) < 1:
      print 'adapt [-ia|c] [-oi|r] input align|class'
      sys.exit(0)

   # get parameter
   sOutput = 'd'
   sInput = 'a'
   for opt, val in opts:
      if opt == '-o':
         sOutput = val
      if opt == '-i':
         sInput = val

   # input
   alignFile = None
   modelFile = None
   align = None
   model = None
   if sInput == 'a':
      alignFile = readAlign(args[1]) 
   elif sInput == 'c':
      modelFile = readModel(args[1])
   else:
      print 'The input format is invalid'
      sys.exit(0)

   for tree in depio.depread(args[0]):
      if alignFile:
         align = alignFile.next()
      dept = dep.CDep(tree)
      reorder(dept, align, model)
      if sOutput == 'd':
         print dept
      elif sOutput == 'r':
         print dept.toRaw()
      elif sOutput == 'i':
         print printOrder(dept)
      else:
         print "The output type is not valid"
         sys.exit(0)
