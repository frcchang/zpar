import sys

STARTMATCH = '###<START>'

def FindStartIdx(file):
   line = file.readline()
   while line:
      if line.startswith(STARTMATCH):
         return int(line[len(STARTMATCH):-1])
      line = file.readline()
   return -1

if __name__ == '__main__':
   file = open(sys.argv[1])
   filepipe = open(sys.argv[2])
   start = FindStartIdx(filepipe)
   bInit = False
   assert start != -1
   for line in file:
      line = line.strip()
      if not line:
         if not bInit: continue
         start = FindStartIdx(filepipe)
         #assert start != -1
      elif line.startswith('<c>') or line.startswith('#'):
         continue
      else:
         if not bInit: bInit = True
         words = line.split()
         for index in (0, -2):
            word = words[index].rsplit('_', 1)
            word[1] = str(int(word[1]) + start)
            words[index] = '_'.join(word)
         print ' '.join(words)
   file.close()
   filepipe.close()
