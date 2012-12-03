import random
import depio
import sys

def randomdep(path, cutoff):
   for d in depio.depread(path):
      n = 0
      for w, p, h, l in d:
         if h != '-1' and random.random() < cutoff:
            d[n][2] = '-1'
         n += 1
      depio.depprint(d)

if __name__ == '__main__':
   path = sys.argv[1]
   cutoff = float(sys.argv[2]) / 100
   randomdep(path, cutoff)
