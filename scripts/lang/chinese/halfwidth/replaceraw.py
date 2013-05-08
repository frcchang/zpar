import replace
import sys

#how many sentences
N=5000

f = open(sys.argv[1])
r = replace.CReplace(sys.argv[2])
n = 0
for line in f:
   if n < N:
      print r.replace(line[:-1])
   else:
      print line[:-1]
   n += 1
f.close()
