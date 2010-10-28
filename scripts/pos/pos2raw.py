#
# Remove tag from corpus. 
# The word format word_tag. 
# Yue Zhang
#

import sys, os

file=open(sys.argv[1])
output=open(sys.argv[2], "w")
n=0
count = 0
line=file.readline().strip()
while line:
   output_line = []
   wordls = line.split(" ")
   for word in wordls:
      lwordtag = word.split("/")
      if len(lwordtag) > 2:
         lword = lwordtag[:-1]
         for sword in lword[:-1]:
            assert sword[-1] == '\\'
      word = "/".join(lwordtag[:-1])
      tag = lwordtag[-1]
      output_line.append(word)
   output.write(" ".join(output_line))
   output.write("\n")
   line=file.readline().strip()
file.close()
output.close()

