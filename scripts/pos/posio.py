import sys
import gzip

def posread(path, sep='/'):
   if path[-3:] == '.gz':
      file = gzip.open(path)
   else:
      file = open(path)
   for line in file:
      retval = [word.split(sep) for word in line.split()]
      for index in range(len(retval)):
         word = retval[index]
         if len(word) > 2:
            retval[index] = [sep.join(word[:-1]), word[-1]]
         if len(retval[index]) != 2:
            print>>sys.stderr, retval[index]
         assert len(retval[index]) == 2
      yield retval
   file.close()

def posprint(sent, sep='/'):
   print ' '.join([sep.join([word[0], word[1]]) for word in sent])

def fromString(s, sep='/'):
   return [word.split(sep) for word in s.split()]

if __name__ == '__main__':
   for sent in posread(sys.argv[1]):
      posprint(sent)
