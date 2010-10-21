import sys
import getopt

def countwords(sent, dRet):
   for word in sent:
      if not word in dRet:
         dRet[word] = 0
      dRet[word] += 1

def readsent(path):
   file = open(path)
   for line in file:
      if not line or line.startswith('#'):
         continue
      yield line.split()
   file.close()

def readtaggedsent(path, sTagSep):
   file = open(path)
   for line in file:
      if not line or line.startswith('#'):
         continue
      yield [word.split(sTagSep)[0] for word in line.split()]
   file.close()

def hint():
   print 'countwords.py [-t] input >output'

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], 't', {})
   except:
      hint()
      sys.exit(1)

   if len(args)<1:
      hint()
      sys.exit(1)

   bTagged = False

   for opt in opts:
      if opt[0] == '-t':
         bTagged = True
      
   dRet = {}
   if bTagged:
      for sent in readtaggedsent(args[0], '|'):
         countwords(sent, dRet)
   else:
      for sent in readsent(args[0]):
         countwords(sent, dRet)

   for key in dRet:
      print key, ':', dRet[key]
