import sys

if __name__ =="__main__":
   file = open(sys.argv[1])
   for line in file:
      line = line.strip()
      if not line.startswith('<'):
         print line
   file.close()
