import sys
import depio

if __name__ == "__main__":
   if len(sys.argv) != 2:
      print "addindex.py input >output"
      sys.exit(1)
   input = sys.argv[1]
   for sent in depio.depread(input):
      index = 0
      for word in sent:
         print '\t'.join([str(index)]+word)
         index += 1      
      print
