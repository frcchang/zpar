import sys

def filtercategorylen(path, lower, upper):
   file = open(path)
   for line in file:
      if not line[:-1] or line.startswith('#'):
         print line[:-1]
         continue
      catsize = len(line.split())
      if catsize>=lower and catsize<=upper:
         print line[:-1]
   file.close()

if __name__ == '__main__':
   path = sys.argv[1]
   lower = int(sys.argv[2])
   upper = int(sys.argv[3])
   filtercategorylen(path, lower, upper)
