import sys
import pprint
import gzip

def filterdict(dict, threshold):
   for word in dict.keys():
      total = 0
      for tag in dict[word]:
         total += dict[word][tag]
      if total < threshold:
         del dict[word]

def readdicttup(path, d):
   for line in path:
      word, tag, count = line.split()
      d[word, tag] = int(count)

def filterdicttup(dDict, threshold):
   dWord = {}
   for word, tag in dDict:
      dWord[word] = dWord.get(word, 0) + dDict[word, tag]
   for word, tag in dDict:
      if dWord[word] >= threshold:
         print word, tag

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])
   threshold = int(sys.argv[2])
#   s = file.read()
#   d = eval(s)
#   filterdict(d, threshold)
#   pprint.pprint(d)
   d = {}; readdicttup(file, d)
   filterdicttup(d, threshold)
   file.close()
