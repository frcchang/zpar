import sys
import depio
import random

_HEAD_INDEX = 2

def countfrag(sent):
   head = 0
   for word in sent:
      if int(word[_HEAD_INDEX]) == -1:
         head += 1
   assert head > 0
   if head == 1:
      return 0
   return 1

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print "depanal.py options input >output"
      print "options: countfrag"
      sys.exit(1)
   option = sys.argv[1]
   input = sys.argv[2]
   if option == "countfrag":
      count = 0
      total = 0
      for sent in depio.depread(input):
         count += countfrag(sent)
         total += 1
      print "%d are fragmented from %d sentences." % (count, total)

