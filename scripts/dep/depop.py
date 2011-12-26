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
            dep_word = deptree[index]
            head_word = deptree[headindex]
            dep = dep_word[0]
            dep_pos = dep_word[1]
            head = head_word[0]
            head_pos = head_word[1]
            head_direction = 'L'
            if headindex > index:
               head_direction = 'R'
            key = (head, dep, head_pos, dep_pos, head_direction)
            if not key in counts:
               counts[key] = 0
            counts[key] += 1

def maxmodifycount(path, condition):
   ret = {}
   for sent in depio.depread(path):
      counts = {}
      for index, word in enumerate(sent):
         head = int(word[2])
         if head != -1 and condition(head, index):
            if not head in counts:
               counts[head]=0
            counts[head] += 1
         for head in counts:
            count = counts[head]
            pos=sent[head][1]
            if count > ret.get(pos, 0):
               ret[pos]=count
   maxcnt=0
   for pos in ret:
      count = ret[pos]
      print pos, ':', count
      if count>maxcnt:
         maxcnt = count
   print 'Overall', ':', maxcnt

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
      for head, dep, head_pos, dep_pos, head_direction in counts:
         print head, head_pos, dep, dep_pos, head_direction, counts[(head, dep, head_pos, dep_pos, head_direction)]
   if option == 'maxleftmodifycount':
      if len(sys.argv) != 3:
         print 'depio.py maxleftmodifycount path'
         sys.exit(0)
      maxmodifycount(sys.argv[2], lambda x,y:x>y)
   if option == 'maxrightmodifycount':
      if len(sys.argv) != 3:
         print 'depio.py maxrightmodifycount path'
         sys.exit(0)
      maxmodifycount(sys.argv[2], lambda x,y:x<y)
