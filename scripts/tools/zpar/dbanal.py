#!/usr/bin/python
# Analyse databases

import sys

file = open(sys.argv[1])
name = "--NONAME--"
max = 0
min = 0
for line in file:
   if not line.strip():
      print "%27s:\t(%.2f, %.2f)" % (name,min,max)
      name = "--NONAME--"
      max = 0
      min = 0
      continue
   if line.find(":") == -1:
      name = line.strip()
      continue
   key, score = tuple(line.rsplit(":", 1))
   nonavg, avg = score.split("/")
   nonavg = float(nonavg.strip())
   if nonavg > max: 
      max = nonavg
   if nonavg < min:
      min = nonavg
file.close()
