import sys

def readpossent(path):
   file = open(path)
   for line in file:
      line = line[:-1]
      sent = line.split()
      yield [word.split('|') for word in sent]
   file.close()


