import sys
import msuper

if __name__ == "__main__":
   d={}
   for s in msuper.msuperread(sys.argv[1]):
      for w in s:
         if not w[1] in d:
            d[w[1]] = set()
         d[w[1]].add(w[2])
   for p in d:
      print p,
      for s in d[p]:
         print s, 
      print
