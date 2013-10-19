#
# Remove tag from corpus. 
# The word format word_tag. 
# Yue Zhang
#

import sys, os

file=open(sys.argv[1])
output=open(sys.argv[2], "w")
sep='/'
if len(sys.argv) > 3:
   sep=sys.argv[3]
n=0
count = 0
line=file.readline().strip()
while line:
   n += 1
   output_line = []
   wordls = line.split(" ")
   for word in wordls:
      lwordtag = word.split(sep)
      if len(lwordtag) > 2:
         lword = lwordtag[:-1]
#         for sword in lword[:-1]:
#            if sword[-1] != '\\':
#               print 'In line', n, 'two \s word.'
#               sys.exit(1)
      word = sep.join(lwordtag[:-1])
      tag = lwordtag[-1]
      output_line.append(word)
   output.write(" ".join(output_line))
   output.write("\n")
   line=file.readline().strip()
file.close()
output.close()

