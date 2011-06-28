import re

g_reHeader = re.compile(r'^[A-Za-z0-9]+ ([0-9]+)$')

def countfeature(path):
   file = open(path)
   count = 0
   for line in file:
      match = g_reHeader.match(line[:-1])
      if match:
         count += int(match.group(1))
   file.close()
   return count

if __name__ == '__main__':
   import sys
   print countfeature(sys.argv[1])
