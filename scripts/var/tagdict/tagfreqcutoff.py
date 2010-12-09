import sys
import gzip
import getopt

def builddic(path, d, w):
   for line in path:
      tup = tuple(line.split())
      assert len(tup) == 3
      d[(tup[0], tup[1])] = int(tup[2])#1
      w[tup[0]] = w.get(tup[0], 0) + int(tup[2])

def tagfreqcutoff(d, w, threshold, bOutputCount):

   for word, tag in d:

      frequ = d[word, tag]

      if frequ > float(w[word]) * threshold:

         if bOutputCount: 
            print word, tag, frequ 
         else: 
            print word, tag

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], 'o:', {})
      assert len(args) == 2
   except:
      print >>sys.stderr, 'wordfreqcutoff.py -o[d|c] input percentage'
      print >>sys.stderr, ' -od: output dictionary'
      print >>sys.stderr, ' -oc: output dictionary and counts'
      print >>sys.stderr, ' closed: leave all closed set tags'
      sys.exit(0)

   if args[0].endswith('.gz'):
      file = gzip.open(args[0])
   else:
      file = open(args[0])
   threshold = 0
   if len(args) ==2:
      threshold = float(args[1])
   bOutputCount = False
   for opt in opts:
      if opt[0] == '-o' and opt[1] == 'c': bOutputCount = True
   d = {}
   w = {}
   builddic(file, d, w)
   tagfreqcutoff(d, w, threshold, bOutputCount)
   file.close()
