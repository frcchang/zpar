import sys
import getopt

def readinput(path):
   file = open(path)
   bStarted = False
   for line in file:
      line = line[:-1]
      if bStarted == False and (not line or line.startswith('#')): continue
      bStarted = True
      yield [word.split('|') for word in line.split()]
   file.close()

def readdict(path, dRet):
   file = open(path)
   for line in file:
      line = line.split()
      assert not line[0] in dRet
      dRet[line[0]] = set()
      assert line[1] == ':'
      index = 2
      while index < len(line):
         dRet[line[0]].add(line[index]); index+=1
         if index==len(line):
            break
         assert line[index] == ','
         index+=1
   file.close()

def readcounts(path, dRet):
   file = open(path)
   for line in file:
      line = line.split()
      assert not line[0] in dRet
      assert line[1] == ':'
      dRet[line[0]] = int(line[2])
   file.close()

def msuper(path, dWordDict, dTagDict, dWordCounts, nCutOff):
   for tagged_word in sent:
      word = tagged_word[0]
      tag = tagged_word[1]
      nWordCount = dWordCounts.get(word, 0)
      if nWordCount >= nCutOff:
         setSuper = dWordDict.get(word, set())
      else:
         setSuper = dTagDict.get(tag, set())
      if len(setSuper) == 0:
         print word, tag, 'doesnot have item'
      assert len(setSuper) > 0
      print word, tag, ' '.join(setSuper)
   print

def hint():
   print 'msuperviadict.py [-t cutoff] input_sent word_dict tag_dict word_counts >output'

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], 't:', {})
   except:
      hint()
      sys.exit(1)

   if len(args)<4:
      hint()
      sys.exit(1)

   nCutOff = 10

   for opt in opts:
      if opt[0] == '-t':
         nCutOff = opt[1]; print nCutOff
   
   dWordDict = {}
   readdict(args[1], dWordDict)

   dTagDict = {}
   readdict(args[2], dTagDict)

   dWordCounts = {}
   readcounts(args[3], dWordCounts)

   for sent in readinput(args[0]):
      msuper(sent, dWordDict, dTagDict, dWordCounts, nCutOff)
