#
# Yue Zhang
#

import sys, os

file_reference=open(sys.argv[1])
file_input=open(sys.argv[2], "w")

line=file_reference.readline()
while line:
   wordls = line.split("\t")
   if len(wordls)>1 :
      file_input.write("_".join([wordls[0], wordls[1]]))
   else:
      file_input.write(wordls[0])
   if line.strip():
      file_input.write(" ")
   line=file_reference.readline()

file_reference.close()
file_input.close()
