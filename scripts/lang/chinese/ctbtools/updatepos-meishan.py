import sys, os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../../pos'))
import posio
import binarize
import getopt
import codecs

def updatepos(path, pospath, sSep, output):

   def updatenode(node, sent):
      if node.type == 'token':
         node.name = sent[0][1]
         sent.pop(0)
      else:
         updatenode(node.left_child, sent)
         if node.right_child:
            updatenode(node.right_child, sent)

   file=codecs.open(path,encoding='utf-8')
   pos_it = posio.posread(pospath, sSep)
   for line in file:
      pos = pos_it.next()
      # get node
      srcnode = binarize.CBinarizedTreeNode()
      srcnode.load(line)
      updatenode(srcnode, pos)
      # prin
      output.write(srcnode.utf8print()+"\n")

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], "l:s:")
   except getopt.GetOptError: 
      print "\nUsage: updatepos-meishan.py parsed_file pos_file output_file\n"
      print "s -- pos separator"
      sys.exit(1)
   if len(args) != 3:
      print "\nUsage: updatepos-meishan.py parsed_file pos_file output_file\n"
      print "s -- pos separator"
      sys.exit(1)
   sSep = '_'
   for opt in opts:
      if opt[0] == '-s':
         sSep = opt[1]
   output=codecs.open(args[2],'w',encoding='utf-8')
   updatepos(args[0], args[1], sSep, output)
