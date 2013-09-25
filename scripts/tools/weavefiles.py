import sys

def weavefiles(paths):
   files = [open(x) for x in paths]
   for line0 in files[0]:
      lines = [x.readline() for x in files[1:]]
      print line0[:-1]
      print ''.join(lines)[:-1]
   [x.close() for x in files]

if __name__ == '__main__':
   weavefiles(sys.argv[1:])
