import sys

def getchunks(path):
   file = open(path)
   r = []
   for line in file:
      r.append(line[:-1])
      if not r[-1]:
         yield r
         r = []
   file.close()

def getnextcategory(path):
   line = path.readline()
   if not line:
      return -1
   while not line[:-1] or line.startswith('#'):
      line = path.readline()
   catsize = len(line.split())
   return catsize

def filtersentlen(path, category_path, lower, upper):
   category_file = open(category_path)
   for chunk in getchunks(path):
      if chunk and chunk[0].startswith('#'): 
         for line in chunk: print line
         continue
      catsize = getnextcategory(category_file)
      assert catsize != -1
      if catsize>=lower and catsize<=upper:
         for line in chunk:
            print line

if __name__ == '__main__':
   path = sys.argv[1]
   category_path = sys.argv[2]
   lower = int(sys.argv[3])
   upper = int(sys.argv[4])
   filtersentlen(path, category_path, lower, upper)
