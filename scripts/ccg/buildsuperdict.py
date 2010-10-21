import sys
import getopt

def buildworddict(sent, dRet):
   for word in sent:
      if not word[0] in dRet:
         dRet[word[0]] = set()
      dRet[word[0]].add(word[2])

def buildtagdict(sent, dRet):
   for word in sent:
      if not word[1] in dRet:
         dRet[word[1]] = set()
      dRet[word[1]].add(word[2])

def readsupersent(path):
   file = open(path)
   for line in file:
      if not line or line.startswith('#'):
         continue
      yield [word.split('|') for word in line.split()]
   file.close()

def hint():
   print 'buildsuperdict.py [-t] input >output'

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], 'wt', {})
   except:
      hint()
      sys.exit(1)

   if len(args)<1:
      hint()
      sys.exit(1)

   bWordDict = True
   bTagDict = False

   for opt in opts:
      if opt[0] == '-t':
         bWordDict = False
         bTagDict = True
      
   dRet = {}
   assert bWordDict and not bTagDict or not bWordDict and bTagDict
   for sent in readsupersent(args[0]):
      if bWordDict:
         buildworddict(sent, dRet)
      else:
         buildtagdict(sent, dRet)

   for key in dRet:
      print key, ':', ' , '.join(dRet[key])
