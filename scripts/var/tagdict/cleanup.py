import sys

def cleanup(path):
   file = open(path)
   for line in file:
      line = line.split()
      int(line[-1]) # just to make sure that it's integerized
      print '%s	%s'%(line[0],line[1])
   file.close()

if __name__ == '__main__':
   cleanup(sys.argv[1])
