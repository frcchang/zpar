import sys
import getopt
import os
import gzip

def readSource(path):
   file = open(path)
   for line in file:
      yield line[:-1].split()
   close(file)

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
   def __init__(self, word, index):
      self.index = index
      self.word = word
      self.next = []

def reorder(source, align):
   source_sentences = readSource(source)
   alignments = readAlign(align)
   # for each sentence...
   for sentence in source_sentences:
      alignment = alignments.next() # iterator
      head = [] # the front non-aligned words
      words = []
      # for each word
      for index in range(len(sentence)):
         align = alignment.get(index, [])
         align.sort() # one word aligned to many
         if align == []:
            if index == len(head):
               head.append(sentence[index])
            else:
               words[-1].next.append(sentence[index])
         else:
            words.append(CWord(sentence[index], align[0]))
      # re-align@
      words = sorted(words, key=lambda x: x.index)
      # print sentence
      output = []
      output.extend(head) # for those unaligned words at the beginning
      for word in words:
         output.append(word.word)
         for next_word in word.next: # for those unaligned words after the word
             output.append(next_word)
      print ' '.join(output)

#========================================

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "o:")
   if len(args) < 1:
      print 'oracle source align'
      sys.exit(0)
#   sOutput = 'd'
#   for opt, val in opts:
#      if opt == '-o':
#         sOutput = val
#
#
#
#
#
#
   reorder(args[0], args[1])
