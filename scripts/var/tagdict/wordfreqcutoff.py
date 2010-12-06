import sys
import pprint
import gzip

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

def filterdicttup(dDict, threshold, lClosedTag):
   dWord = {}
   for word, tag in dDict:
      dWord[word] = dWord.get(word, 0) + dDict[word, tag]
   for word, tag in dDict:
      if dWord[word] >= threshold or tag in lClosedTag:
         print word, tag, dDict[word, tag]

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])

   lClosedTag = ["$", "``", "''", "-LRB-", "-RRB-", ",", ".", ":", "#", 
                 "CC", "DT", "EX", "IN", "MD", "PDT", "POS", "PRP", "PRP$",
                 "TO", "WDT", "WP", "WP$", "WRB"]

   threshold = int(sys.argv[2])

   if len(sys.argv) == 4:
      assert sys.argv[3] == 'closed'
   else:
      lClosedTag = []

   d = {}; readdicttup(file, d)
   filterdicttup(d, threshold, lClosedTag)
   file.close()
