import sys
import getopt
import os
import gzip

def readSource(path):
   file = open(path)
   for line in file:
      yield line[:-1].split()
   file.close()

def readAlign(path):
   retval = {}
   n=0
   file = gzip.open(path)
   for line in file:
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

class CWord(object):
   def __init__(self, word, align):
      self.align = align
      self.word = word
      self.next = []
      self.index = -1
      if self.align:
         self.index = self.align[0]

def filter(source, align):
   alignments = readAlign(align)
   # for each sentence...
   for sentence in readSource(source):
      alignment = alignments.next() # iterator
      heads = [] # the front non-aligned words
      words = []
      # for each word
      for index in range(len(sentence)):
         align = alignment.get(index, [])
         align.sort() # one word aligned to many
         if align == []:
            if index == len(heads):
               heads.append(CWord(sentence[index], []))
            else:
               words[-1].next.append(CWord(sentence[index], []))
         else:
            words.append(CWord(sentence[index], align))
      # re-align@
      words = sorted(words, key=lambda x: x.index)
      # print sentence
      output = []
      for head in heads:
         output.append(head)
      for word in words:
         output.append(word)
         for next_word in word.next: # for those unaligned words after the word
             output.append(next_word)
      # calculate crossinglinks
      total = 0
      cross = 0
      for i in range(len(output)):
         for j in range(i+1, len(output)):
            for ai in output[i].align:
               for aj in output[j].align:
                  total += 1
                  if ai > aj:
                     cross += 1
      if cross > float(total)*0.2:
         print 0, ' '.join(sentence)
      else: print ' '.join(sentence)

#========================================

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "o:")
   if len(args) < 1:
      print 'filtero source align'
      sys.exit(0)
   filter(args[0], args[1])
