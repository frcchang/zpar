import sys
import re

def vnformat(path):
   rex = re.compile('[0-9]+')
   file = open(path)
   for line in file:
      line = line[:-1].split()
      while not rex.match(line[1]):
         line[0] = '%s_%s' %(line[0],line[1])
         line.pop(1)
      for s in line[1:]: assert int(s) != -1
      print '[%s] : [ %s ]' % (line[0],' , '.join(line[1:]))
   file.close()

if __name__ == '__main__':
   vnformat(sys.argv[1])
