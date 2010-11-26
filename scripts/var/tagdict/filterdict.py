import sys
import pprint
import gzip

def filterdict(dict, threshold):
   for word in dict:
      total = 0
      for tag in dict[word]:
         total += dict[word][tag]
      if total < threshold:
         del dict[word]

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])
   threshold = int(sys.argv[2])
   s = file.read()
   d = eval(s)
   filterdict(d, threshold)
   pprint.pprint(d)
