import sys

# stagged
def msuperread(path):
   file = open(path)
   for line in file:
      line = line.split()
      retval = []
      for word in line:
         retval.append(word.split('|'))
      yield retval
   file.close()

# msuper output
def superread(path):
   file = open(path)
   tokens = []
   for line in file:
      line = line[:-1]
      line = line.strip()
      if not line:
         yield tokens
         tokens = []
         continue
      line = line.split()
      n = int(line[2])
      t = []
      for i in range(n):
         t.append(line[3+2*i])
      tokens.append([line[0], line[1], t[:]])
   file.close()

if __name__ == '__main__':
   for s in superread(sys.argv[1]):
      print s
