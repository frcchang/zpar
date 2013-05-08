import sys
import depio
import replace

# how many changed
N=10000

r = replace.CReplace(sys.argv[2])
n = 0
for sent in depio.depread(sys.argv[1]):
   if n < N:
      for i in range(len(sent)):
         sent[i][0] = r.replace(sent[i][0])
   n += 1
   depio.depprint(sent)
