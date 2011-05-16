import sys
import pprint
import gzip
import getopt
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../pos"))

def closedtagdict(path, dDict):
   for sent in posio.posread(path):
      for word in sent:
         if not word[0] in dDict:
            dDict[word[0]] = {}
         if not word[1] in dDict[word[0]]:
            dDict[word[0]][word[1]] = 0
         dDict[word[0]][word[1]] += 1

def filterdict(dict, threshold):
   for word in dict:
      total = 0
      for tag in dict[word]:
         total += dict[word][tag]
      if total < threshold:
         del dict[word]

def readdicttup(path, d):
   for line in path:
      word, tag, count = line.split()
      d[word, tag] = int(count)

def filterdicttup(dDict, threshold, lClosedTag, bOutputCount):
   dWord = {}
   for word, tag in dDict:
      dWord[word] = dWord.get(word, 0) + dDict[word, tag]
   for word, tag in dDict:
      if dWord[word] >= threshold or tag in lClosedTag:
         if bOutputCount:
            print word, tag, dDict[word, tag]
         else:
            print word, tag

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], 'o:', {})
   print >>sys.stderr, 'wordfreqcutoff.py -o[d|c] input [closed]'
   print >>sys.stderr, ' -od: output dictionary'
   print >>sys.stderr, ' -oc: output dictionary and counts'
   print >>sys.stderr, ' closed: keep all closed set tags'

   if args[0].endswith('.gz'):
      file = gzip.open(args[0])
   else:
      file = open(args[0])

#   lClosedTag = [define]

   threshold = int(args[1])

   if len(args) == 4:
      assert args[2] == 'closed'
   else:
      lClosedTag = []

   bOutputCount = False
   for opt in opts:
      if opt[0] == '-o' and opt[1] == 'c':
         bOutputCount = True

   d = {}; readdicttup(file, d)
   filterdicttup(d, threshold, lClosedTag, bOutputCount)
   file.close()
