import sys

def filterdeplen(path, lower, upper):
   file = open(path)
   for line in file:
      line = line[:-1]
      if line.startswith('#') or line.startswith('<c>') or not line:
         print line
         continue
      words = line.split()
      #assert len(words)==5
      words[0] = words[0].split('_')
      assert len(words[0]) == 2
      pos1 = int(words[0][1])
      words[3] = words[3].split('_')
      assert len(words[3]) == 2
      pos2 = int(words[3][1])
      size = abs(pos2 - pos1)
      if size >= lower and size<=upper:
         print line
   file.close()

if __name__ == '__main__':
   path = sys.argv[1]
   lower = int(sys.argv[2])
   upper = int(sys.argv[3])
   filterdeplen(path, lower, upper)
