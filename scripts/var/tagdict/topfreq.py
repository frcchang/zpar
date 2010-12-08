import sys
import gzip

def builddic(path, d, w):
   for line in path:
      tup = tuple(line.split())
      assert len(tup) == 3
      d[(tup[0], tup[1])] = int(tup[2])#1
      w[tup[0]] = w.get(tup[0], int(tup[2]))
      if int(tup[2]) > w[tup[0]]:
         w[tup[0]] = int(tup[2])

def tagfreqcutoff(d, w):
   output = {}
   for word, tag in d:
      frequ = d[word, tag]
      if frequ == w[word]:
         output[word] = tag
   return output

def printdic(d):
   for word in d:
      print word, d[word]

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])
   d = {}
   w = {}
   builddic(file, d, w)
   d = tagfreqcutoff(d, w)
   printdic(d)
   file.close()
