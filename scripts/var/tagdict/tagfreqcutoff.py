import sys
import gzip

def builddic(path, d, w):
   for line in path:
      tup = tuple(line.split())
      assert len(tup) == 3
      d[(tup[0], tup[1])] = int(tup[2])#1
      w[tup[0]] = w.get(tup[0]) + 1

def tagfreqcutoff(d, w, threshold):
   for word, tag in d:
      frequ = d[word, tag]
      if frequ >= float(w[word]) * threshold:
         print word, tag

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])
   threshold = 1
   if len(sys.argv) ==3:
      threshold = int(sys.argv[2])
   d = {}
   w = {}
   builddic(file, d, w)
   tagfreqcutoff(d, w, threshold)
   file.close()
