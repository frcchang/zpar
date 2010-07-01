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

def headdepcount(path, counts):
   for deptree in depio.depread(input):
      for index in range(len(deptree)):
         headindex = int(deptree[index][2])
         if headindex != -1:
            dep = deptree[index][0]
            head = deptree[headindex][0]
            if not (head, dep) in counts:
               counts[(head, dep)] = 0
            counts[(head, dep)] += 1

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print "depop.py options input >output"
      sys.exit(1)
   option = sys.argv[1]
   input = sys.argv[2]
   if option == "remove":
      for sent in depio.depread(input):
         if len(sent) <= 1:
            continue
         index = random.randint(0, len(sent)-1)
         depio.depprint(removeword(sent, index))
   if option == "headdepcount":
      counts = {}
      headdepcount(input, counts)
      for head, dep in counts:
         print head, dep, counts[(head, dep)]
