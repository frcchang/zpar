import sys

def msuperread(path):
   file = open(path)
   for line in file:
      line = line.split()
      retval = []
      for word in line:
         retval.append(word.split('|'))
      yield retval
   file.close()
