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

def listanarc(sent, arc):
   for word in sent:
      dep_word = word[0]
      dep_pos = word[1]
      head_index = int(word[2])
      if head_index == -1:
         continue
      head_pos = sent[head_index][1]
      head_word = sent[head_index][0]
      if not (head_word, head_pos, dep_word, dep_pos) in arc:
         arc[(head_word, head_pos, dep_word, dep_pos)] = 0
      arc[(head_word, head_pos, dep_word, dep_pos)] += 1

def findpunct(sent, punct):
   for word in sent:
      dep_word = word[0]
      dep_pos = word[1]
      if dep_pos == ',' or dep_pos == '.' or dep_pos == '-LRB-' or dep_pos == '-RRB-' or dep_pos == ':' or dep_pos == '``' or dep_pos == '"' or dep_pos == '#' or dep_pos == '$':
         punct.add(dep_word)

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print "depanal.py options input >output"
      print "options: countfrag listarc findpunct"
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
   elif option == "listarc":
      arc = {}
      for sent in depio.depread(input):
         listanarc(sent, arc)
      for anarc in arc:
         print '	'.join(anarc), arc[anarc]
   elif option == "findpunct":
      punct = set()
      for sent in depio.depread(input):
         findpunct(sent, punct)
      for punc in punct:
         print punc

