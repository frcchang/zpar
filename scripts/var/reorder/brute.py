import sys
import getopt
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../dep"))
import depio
import dep
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../ml"))
import orng
import gzip

#========================================

def readModel(path, bTrain):
   model = orng.COrangeWriter(path, bTrain)
   return model

def readAlign(path):
   retval = {}
   n=0
   file = gzip.open(path)
   for line in file:
      if not line.strip():
         continue
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

def __PrintOrderForNode(node):
   retval = []
   for left_child in node.left_children:
      retval.extend(__PrintOrderForNode(left_child))
   retval.append(node.extra)
   for right_child in node.right_children:
      retval.extend(__PrintOrderForNode(right_child))
   return retval

# print the original index of the words in the current order
def printOrder(tree):
   retlst = __PrintOrderForNode(tree.root)
   retval = ' '.join(map(sre, retlst))
   return retval

def listOrder(tree):
   retlst = __PrintOrderForNode(tree.root)
   return retlst

#========================================

def __updateIndexForNode(node, index):
   node.extra = node.word_index
   node.word_index = index
   return index+1

def __updateIndexForTree(node, index):
   for left_child in node.left_children:
      index = __updateIndexForTree(left_child, index)
   index = __updateIndexForNode(node, index)
   for right_child in node.right_children:
      index = __updateIndexForTree(right_child, index)
   return index

# record the original index into 'extra'
# post-processing after the whole reordering is done
# word_index field updated with new order
def updateIndex(tree):
   __updateIndexForTree(tree.root, 0)

#========================================

def indicesRange(node):
   return range(node.leftmost_leaf.word_index, node.rightmost_leaf.word_index+1)

def getAlignedNodes(node, align, align_cache):
   ret = []
   for i in indicesRange(node):
      ret.append(getAlignedNode(i, align, align_cache))
   return ret

def getAlignedNode(i, align, align_cache):
   if align_cache[i] == -2:
      j = align.get(i, [])
      if j:
         align_cache[i] = j[0]
      else:
         align_cache[i] = -1
   return align_cache[i]

def onTheLeft(x, y):
   left = 0
   right = 0
   for i in x:
      for j in y:
         if i != -1 and j != -1 and i != j:
            if i < j:
               left += 1
            else:
               right += 1
   return left > right

def reorderNode(node, align, model, align_cache, bDebug):
   if not node.pos in ['VV', 'NN', 'NR', 'LC']:
      return
   src = []
   tgt = []
   for left_child in node.left_children:
      left_child.extra = True
      reorderNode(left_child, align, model, align_cache, bDebug)
      src.append((left_child, getAlignedNodes(left_child, align, align_cache)))
   src.append((node, [getAlignedNode(node.word_index, align, align_cache)]))
   for right_child in node.right_children:
      right_child.extra = False
      reorderNode(right_child, align, model, align_cache, bDebug)
      src.append((right_child, getAlignedNodes(right_child, align, align_cache)))

   for src_node, src_aligned_nodes in src:
      n = 0
      for tgt_node, tgt_aligned_nodes in tgt:
         if onTheLeft(src_aligned_nodes, tgt_aligned_nodes):
            break
         n += 1
      tgt.insert(n, (src_node, src_aligned_nodes))

   head_index = node.word_index
   left_children = []
   right_children = []
   left = True
   for tgt_node, tgt_aligned_node in tgt:
      if left:
         if tgt_node.word_index == head_index:
            left = False
         else:
            left_children.append(tgt_node)
      else:
         if tgt_node.extra:
            if tgt_node.pos in ['DEC', 'DEG']:
               tgt_node.token = tgt_node.token + '*'
               tgt_node.right_children.extend(tgt_node.left_children)
               tgt_node.left_children = []
         right_children.append(tgt_node)
   node.left_children = left_children
   node.right_children = right_children

def reorder(tree, align, model, bDebug):
#   assert (align != None and model.bTrain) or (align == None and not model.bTrain)
   if align:
      align_cache = [-2]*len(tree.nodes)
      reorderNode(tree.root, align, model, align_cache, bDebug)
   updateIndex(tree)

#========================================


#========================================

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "o:i:d")
   if len(args) < 1:
      print 'adapt [-ia|c] [-oi|r|p|d] [-d] input model [align]'
      sys.exit(0)

   # get parameter
   sOutput = 'd'
   sInput = 'a'
   bDebug = False
   for opt, val in opts:
      if opt == '-o':
         sOutput = val
      if opt == '-i':
         sInput = val
      if opt == '-d':
         bDebug = True

   # input
   alignFile = None
   align = None
   model = None
   if sInput == 'a':
      if len(args) != 3:
         print "The alignment file must be provided with -ia"
         sys.exit(0)
      model = readModel(args[1], True)
      alignFile = readAlign(args[2]) 
   elif sInput == 'c':
      model = None #readModel(args[1], False)
   else:
      print 'The input format is invalid'
      sys.exit(0)

   for tree in depio.depread(args[0]):
      if alignFile:
         align = alignFile.next()
         if align == None:
            depio.depprint(tree)
         assert align != None
      dept = dep.CDep(tree)
      reorder(dept, align, model, bDebug)
      if sOutput == 'd':
         print dept
      elif sOutput == 'p':
         print dept.toPOS()
      elif sOutput == 'r':
         print dept.toRaw()
      elif sOutput == 'i':
         print printOrder(dept)
      else:
         print "The output type is not valid"
         sys.exit(0)
   if alignFile:
      model.train()
