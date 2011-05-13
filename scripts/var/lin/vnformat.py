import sys

def vnformat(path):
   file = open(path)
   for line in file:
      line = line[:-1].split()
      print '[%s] : [ %s ]' % (line[0],' , '.join(line[1:]))
   file.close()

if __name__ == '__main__':
   vnformat(sys.argv[1])
