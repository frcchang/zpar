import sys, os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../../pos'))
import posio
import binarize

def updatepos(path, pospath):

   def updatenode(node, sent):
      if node.type == 'token':
         node.name = sent[0][1]
         sent.pop(0)
      else:
         updatenode(node.left_child, sent)
         if node.right_child:
            updatenode(node.right_child, sent)

   file = open(path)
   pos_it = posio.posread(pospath)
   for line in file:
      pos = pos_it.next()
      # get node
      srcnode = binarize.CBinarizedTreeNode()
      srcnode.load(line)
      updatenode(srcnode, pos)
      # prin
      print srcnode

if __name__ == '__main__':
   updatepos(sys.argv[1], sys.argv[2])
