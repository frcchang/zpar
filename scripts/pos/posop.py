import sys
import getopt
import pprint

def tagdict(path, dDict):
   import posio
   for sent in posio.posread(path):
      for word in sent:
         if not word[0] in dDict:
            dDict[word[0]] = {}
         if not word[1] in dDict[word[0]]:
            dDict[word[0]][word[1]] = 0
         dDict[word[0]][word[1]] += 1

def tagdicttup(path, dDict):
   import posio
   for sent in posio.posread(path):
      for word in sent:
         dDict[(word[0], word[1])] = dDict.get((word[0], word[1]), 0) + 1

def readdicttup(path, dDict):
   for line  in path:
      line = line.split()
      assert len(line) == 3
      dDict[line[0], line[1]] = int(line[2])

def pprintdicttup(dDict):
   pprint.pprint(dDict)

def printdicttup(dDict):
   for word, tag in dDict:
      print word, tag, dDict[word, tag]

if __name__ == '__main__':
   if len(sys.argv) < 2:
      print 'posop.py option argument'
      sys.exit(0)
   option = sys.argv[1]
   if option == 'tagdict':
      if len(sys.argv) < 3:
         print 'posop.py tagdict path [dict_path]'
         sys.exit(0)
      dDict = {}
      if len(sys.argv) == 4:
         file_dic = open(sys.argv[3])
         readdicttup(file_dic, dDict)
         file_dic.close()
      tagdicttup(sys.argv[2], dDict)
      printdicttup(dDict)
      
