import sys
import depio
import random

_HEAD_INDEX = 2

def gethead(node):
   return int(node[_HEAD_INDEX])

def sethead(node, head):
   node[_HEAD_INDEX] = str(head)

def removeword(sent, index):
   ret = []
   for id, word in enumerate(sent):
      if id != index:
         head = gethead(word)
         if head > index:
            sethead(word, head-1)
         elif head == index:
            sethead(word, -1)
         ret.append(word)
   return ret

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print "depop.py options input >output"
      sys.exit(1)
   option = sys.argv[1]
   input = sys.argv[2]
   if option == "remove":
      for sent in depio.depread(input):
         index = random.randint(0, len(sent)-1)
         depio.depprint(removeword(sent, index))
