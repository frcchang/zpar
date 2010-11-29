import sys
import pprint
import gzip

def tagrel(dict):
   retval = {}
   for word in dict:
      maxtag_count = 0
      maxtag = None
      for tag in dict[word]:
         if dict[word][tag] > maxtag_count:
            maxtag_count = dict[word][tag]
            maxtag = tag
      for tag in dict[word]:
         if tag != maxtag:
            retval[(maxtag, tag)] = retval.get((maxtag,tag),0) + 1
   return retval

if __name__ == '__main__':
   if sys.argv[1].endswith('.gz'):
      file = gzip.open(sys.argv[1])
   else:
      file = open(sys.argv[1])
   s = file.read()
   d = eval(s)
   r = tagrel(d)
   pprint.pprint(r)
