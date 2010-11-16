import sys

def posread(path, sep='/'):
   file = open(path)
   for line in file:
      yield [word.split(sep) for word in line.split()]
   file.close()

def posprint(sent, sep='/'):
   print ' '.join([sep.join([word[0], word[1]]) for word in sent])

if __name__ == '__main__':
   for sent in posread(sys.argv[1]):
      posprint(sent)
