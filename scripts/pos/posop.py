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
         dDict = eval(file_dic.read())
         file_dic.close()
      tagdict(sys.argv[2], dDict)
      assert pprint.isreadable(dDict)
      pprint.pprint(dDict)
      #for word in dDict:
         #print word, ':', ', '.join(['%s (%d)' % (tag, dDict[word][tag]) for tag in dDict[word]])
      
