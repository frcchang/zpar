import sys

def countlines(file):
   """count the number of lines in a file"""
   path = open(file)
   total = 0
   for line in path:
      total += 1
   path.close()
   return total

def filterlines(path, dLines, bConsume=False):
   """ filter lines from path with lines in dLines"""
   file = open(path)
   for line in file:
      line = line.strip()
      if line in dLines:
         if bConsume:
            dLines.delete(line)
      else:
         print line
   file.close()

def getchunks(path):
   """split file into chunks, each of which ends with an empty line"""
   file = open(path)
   r = []
   for line in file:
      r.append(line[:-1])
      if not r[-1]:
         yield r
         r = []
   file.close()

def selectchunks(path, lChunks):
   """selectively print chunks from file, according to lChunks"""
   index = 0
   for lChunk in getchunks(path):
      if index in lChunks:
         for line in lChunk:
            print line
      index += 1

def excludechunks(path, lChunks, lReplaceChunk):
   """selectively print chunks from file, excluding those from lChunks and replacing them with the lReplaceChunk item"""
   index = 0
   for lChunk in getchunks(path):
      if index in lChunks:
         for line in lReplaceChunk:
            print line
      else:
         for line in lChunk:
            print line
      index += 1

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
   elif command == 'selectchunk':
      if not len(sys.argv) == 4:
         print 'fileutils.py selectchunk path chunks(in python list format without space 0 start)'
         sys.exit(0)
      path = sys.argv[2]
      chunks = eval(sys.argv[3])
      assert type(chunks) == list
      selectchunks(path, chunks)
   elif command == 'excludechunk':
      if not len(sys.argv) == 5:
         print 'fileutils.py selectchunk path chunks(in python list format without space 0 start) replacechunk(in python list format one element perline; empty list[] means no replacing)'
         sys.exit(0)
      path = sys.argv[2]
      chunks = eval(sys.argv[3])
      replace = eval(sys.argv[4])
      assert type(chunks) == list and type(replace) == list
      excludechunks(path, chunks, replace)
