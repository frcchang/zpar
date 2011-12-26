import os

def merge_ptb(path, begin, end):
   for index in range(begin, end+1):
      sIndex = '%02d'%index
      onePath=os.path.join(path,sIndex)
      fileList=os.listdir(onePath)
      fileList.sort()
      for file in fileList:
         if file.startswith('WSJ'):
            file = os.path.join(onePath, file)
#            print file
            file = open(file)
            for line in file:
               line = line[:-1]
               print line
            file.close()

if __name__ == '__main__':
   import sys
   if not len(sys.argv) in (3, 4):
      print 'merge_ptb.py path start [end]'
   path = sys.argv[1]
   begin = int(sys.argv[2])
   if len(sys.argv) == 4:
      end = int(sys.argv[3])
   else: 
      end = begin
   merge_ptb(path, begin, end)
