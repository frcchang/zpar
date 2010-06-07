import sys

def countlines(file):
   path = open(file)
   total = 0
   for line in path:
      total += 1
   close(path)
   return total

def filterlines(path, dLines, bConsume=False):
   file = open(path)
   for line in file:
      line = line.strip()
      if line in dLines:
         if bConsume:
            dLines.delete(line)
      else:
         print line
   file.close()

if __name__ == '__main__':
   if len(sys.argv) < 2:
      print 'fileutils.py command'
      sys.exit(1)
   command = sys.argv[1]
   if command == 'filter':
      if len(sys.argv) < 4:
         print 'fileutils.py filter input exclude >output'
         sys.exit(0)
      dLines = {}
      file = open(sys.argv[3])
      for line in file:
         dLines[line.strip()] = 1
      file.close()
      filterlines(sys.argv[2], dLines, True)
      assert dLines=={} # every exclude line in file
