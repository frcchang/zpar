#
# switch the romanian treebank to dependency format of zpar
# Yue Zhang
# 2010.3
#
import xml.dom.minidom as minidom
import sys
import getopt

def getText(node):
    count = 0
    rc = ""
    for node in node.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + node.data
            count += 1
    assert count == 1
    return rc.encode("UTF-8")

def getNode(node, name):
   rcs = node.getElementsByTagName(name)
   assert len(rcs) == 1
   rc = rcs[0]

   return rc

def getHead(syn, count):
   head=int(getText(getNode(syn, 'head')))
   if head == count+1:
      head = 0
   # from 0 record
   head = head-1
   return str(head)

def dom2dep(node, bLabeled):
   count = 0
#   tokens = node.getElementsByTagName('tok')
#   for token in tokens:
   for token in node.childNodes: 
      if token.nodeName == 'tok':
         count += 1
   index = 0
   for token in node.childNodes: 
      if token.nodeName == 'tok':
         index += 1
         assert int(getText(getNode(token, 'ordno'))) == index
         syn = getNode(token, 'syn')
         if bLabeled:
            word = [getText(getNode(token, 'orth')), 
                    getText(getNode(token, 'ctag')).replace(" ", "_"), 
                    getHead(syn, count),
                    getText(getNode(syn, 'reltype')).replace(" ", "_")]
         else:
            word = [getText(getNode(token, 'orth')), 
                    getText(getNode(token, 'ctag')).replace(" ", "_"), 
                    getHead(syn, count) ]
         print '\t'.join(word)
   print
   return

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "u")
   if len(args) != 1:
      print "usage: python dgaxml2dep input >output"
   bLabeled = True
   for opt, value in opts:
      if opt == "-u":
         bLabeled = False
   file = open(args[0])
   rtnode = minidom.parse(file)
   file.close()
   assert rtnode.nodeName == "#document" 
   for docnode in rtnode.childNodes:
      assert docnode.nodeName == 'DGAdoc'
      for node in docnode.childNodes:
         assert node.nodeName == 's' or node.nodeName == '#text'
         if node.nodeName == 's':
            dom2dep(node, bLabeled)
