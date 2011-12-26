def readsymbols(path):
   file = open(path)
   for line in file:
      line = line.split()
      for symbol in line:
        yield symbol
   file.close()

if __name__ == '__main__':
   import sys
   for symbol in readsymbols(sys.argv[1]):
     print symbol
