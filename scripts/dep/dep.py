import sys
import depio

class CDepNode(object):
   __slots__ = ['token', 'pos', 'parent', 'left_children', 'right_children', 'leftmost_leaf', 'rightmost_leaf', 'size', 'word_index', 'label', 'extra']
   def __init__(self):
      self.left_children = [  ]
      self.right_children = [  ]
  
class CDep(object):
   def __init__(self, lWords):
      nodes = [CDepNode() for word in lWords]
      index = 0
      for word in lWords:
         node = nodes[index]
         node.token = word[0]
         node.pos = word[1]
         parent = int(word[2])
         if parent == -1:
            self.root = node
            node.parent = None
         else:
            node.parent = nodes[parent]
            if index < parent:
               node.parent.left_children.append(node)
            else:
               assert index > parent
               node.parent.right_children.append(node)
         node.word_index = index
         if len(word) > 3:
            node.label = word[3]
         else:
            node.label = None
         #
         #
         #
         index += 1
      self.updateSize(self.root)
      self.nodes = nodes

   def updateSize(self, node):
      node.size=0
      
      for l_child in node.left_children:
         self.updateSize(l_child)
         node.size += l_child.size
      node.size += 1
      for r_child in node.right_children:
         self.updateSize(r_child)
         node.size += r_child.size

      node.leftmost_leaf = node
      node.rightmost_leaf = node
      if node.left_children:
         node.leftmost_leaf = node.left_children[0].leftmost_leaf
      if node.right_children:
         node.rightmost_leaf = node.right_children[-1].rightmost_leaf
      assert node.rightmost_leaf.word_index-node.leftmost_leaf.word_index+1==node.size

   def printnode(self, node):
      parent_index = -1
      if node.parent != None:
         parent_index = node.parent.word_index
      if node.label:
         return '%s\t%s\t%d\t%s\n' % (node.token, node.pos, parent_index, node.label)
      else:
         return '%s\t%s\t%d\n' % (node.token, node.pos, parent_index)

   def printtree(self, node):
      retval = ''
      for left_child in node.left_children:
         retval += self.printtree(left_child)
      retval += self.printnode(node)
      for right_child in node.right_children:
         retval += self.printtree(right_child)
      return retval

   def __str__(self):
      return self.printtree(self.root)
   
   def printrawnode(self, node):
      retval = []
      for left_child in node.left_children:
         retval.extend(self.printrawnode(left_child))
      retval.append(node.token)
      for right_child in node.right_children:
         retval.extend(self.printrawnode(right_child))
      return retval

   def printposnode(self, node):
      retval = []
      for left_child in node.left_children:
         retval.extend(self.printposnode(left_child))
      retval.append([node.token, node.pos])
      for right_child in node.right_children:
         retval.extend(self.printposnode(right_child))
      return retval

   def toRaw(self):
      return ' '.join(self.printrawnode(self.root))

   def toPOS(self):
      return ' '.join(['|'.join(word) for word in self.printposnode(self.root)])

if __name__ == '__main__':
   for words in depio.depread(sys.argv[1]):
      dep = CDep(words)
      print dep
