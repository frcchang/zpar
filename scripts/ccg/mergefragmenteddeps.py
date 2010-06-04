import sys

STARTMATCH = '###<START>'
ENDMATCHES = '<END>'

def FindStartAndEndIndex(file, default):
   line = file.readline()
   while line:
      if line.startswith(STARTMATCH):
         start, end = line[:-1].split()
         assert end.startswith(ENDMATCHES)
         return int(start[len(STARTMATCH):]), int(end[len(ENDMATCHES):])
      line = file.readline()
   return default, default

if __name__ == '__main__':
   file = open(sys.argv[1])
   filepipe = open(sys.argv[2])
   filecats = open(sys.argv[3])
   cat_line = filecats.readline()
   cats = [word.split('|')[0] for word in cat_line.split()[1:]]
   filecats.close()
   nEmptyLineCount = 0
   start, end = FindStartAndEndIndex(filepipe, -1)
   nInit = 0 # uninitialized
   bFragmented = True
   if start == -1:
      bFragmented = False
      start = 0
   bNothingFromBinGenerator = False
   if start == end:
      nEmptyLineCount -= 1
   lines = []
   for line in file:
      line = line.strip()
      if not line:
         if nInit == 1: # empty line after initial comments
            nInit = 2 # skip it
            continue
         if nInit == 0: # empty line before any comment
            nInit = 2 # started
         if nEmptyLineCount > 0:
            bNothingFromBinGenerator = True
            break
         nEmptyLineCount += 1
         if bFragmented: 
            start, end = FindStartAndEndIndex(filepipe, -1)
            if start==end:
               nEmptyLineCount -= 1
      elif line.startswith('<c>') or line.startswith('#'):
         assert not line.startswith('<c>')
         if line.startswith('#') and nInit==0: nInit=1 # start with #
         continue
      else:
         assert start != -1
         if nInit < 2: nInit = 2 # initialized
         nEmptyLineCount = 0
         words = line.split()
         for index in (0, 3):
            word = words[index].rsplit('_', 1)
            if len(word) < 2:
               print >>sys.stderr, line
            if not word[0] == cats[int(word[1]) + start - 1]:
               print word[0], cats, int(word[1]), start-1
            assert word[0] == cats[int(word[1]) + start - 1]
            word[1] = str(int(word[1]) + start)
            words[index] = '_'.join(word)
         lines.append(' '.join(words))
#   if bNothingBinGenerator: 
#      print
##   else: 
   for line in lines: 
      print line
   file.close()
   filepipe.close()
