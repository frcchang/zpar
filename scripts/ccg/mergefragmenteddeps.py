import sys

STARTMATCH = '###<START>'

def FindStartIdx(file, default):
   line = file.readline()
   while line:
      if line.startswith(STARTMATCH):
         return int(line[len(STARTMATCH):-1])
      line = file.readline()
   return default

if __name__ == '__main__':
   file = open(sys.argv[1])
   filepipe = open(sys.argv[2])
   start = FindStartIdx(filepipe, -1)
   bInit = False
   bFragmented = True
   if start == -1:
      bFragmented = False
      start = 0
   for line in file:
      line = line.strip()
      if not line:
         if not bInit: continue
         if bFragmented: start = FindStartIdx(filepipe, -1)
      elif line.startswith('<c>') or line.startswith('#'):
         continue
      else:
         assert start != -1
         if not bInit: bInit = True
         words = line.split()
         for index in (0, 3):
            word = words[index].rsplit('_', 1)
            if len(word) < 2:
               print >>sys.stderr, line
            word[1] = str(int(word[1]) + start)
            words[index] = '_'.join(word)
         print ' '.join(words)
   file.close()
   filepipe.close()
