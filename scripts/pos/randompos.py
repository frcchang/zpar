import sys
import posio
import random

def randompos(path, cutoff):
   for p in posio.posread(path):
      n = 0
      for w, t in p:
         if random.random() < cutoff:
            p[n][1] = '_NONE-'
         n += 1
      posio.posprint(p)

if __name__ == '__main__':
   path = sys.argv[1]
   cutoff = float(sys.argv[2]) / 100
   randompos(path, cutoff)
