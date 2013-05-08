import sys
import posio
import replace

# how many sentences
N=5000

f = sys.argv[1]
r = replace.CReplace(sys.argv[2])
i = 0
for sent in posio.posread(f, '_'):
   if i < N:
      for n in range(len(sent)):
         sent[n][0] = r.replace(sent[n][0])
   i += 1; posio.posprint(sent, '_')
