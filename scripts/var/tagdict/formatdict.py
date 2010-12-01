import sys
import gzip

def formatdict(dDict, threshold):
   for word in dDict:
      total = 0
      for tag in dDict[word]:
         total += dDict[word][tag]
      if total >= threshold:
         for tag in dDict[word]:
            print word, tag, dDict[word][tag]

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])
   threshold = 1
   if len(sys.argv) ==3:
      threshold = int(sys.argv[2])
   s = file.read()
   d = eval(s)
   formatdict(d, threshold)
   file.close()
