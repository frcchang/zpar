import sys
import getopt

def tagdict(path, dDict):
   import posio
   for sent in posio.posread(path):
      for word in sent:
         if not word[0] in dDict:
            dDict[word[0]] = {}
         if not word[1] in dDict[word[0]]:
            dDict[word[0]][word[1]] = 0
         dDict[word[0]][word[1]] += 1

if __name__ == '__main__':
   if len(sys.argv) < 2:
      print 'posop.py option argument'
      sys.exit(0)
   option = sys.argv[1]
   if option == 'tagdict':
      if len(sys.argv) < 3:
         print 'posop.py tagdict path'
         sys.exit(0)
      dDict = {}
      tagdict(sys.argv[2], dDict)
      for word in dDict:
         print word, ':', ', '.join(['%s (%d)' % (tag, dDict[word][tag]) for tag in dDict[word]])
      
