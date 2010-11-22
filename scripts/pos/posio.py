import sys

def posread(path, sep='/'):
   file = open(path)
   for line in file:
      retval = [word.split(sep) for word in line.split()]
      for index in range(len(retval)):
         word = retval[index]
         if len(word) > 2:
            retval[index] = [sep.join(word[:-1]), word[-1]]
      yield retval
   file.close()

def posprint(sent, sep='/'):
   print ' '.join([sep.join([word[0], word[1]]) for word in sent])

if __name__ == '__main__':
   for sent in posread(sys.argv[1]):
      posprint(sent)
