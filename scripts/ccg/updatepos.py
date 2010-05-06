import sys

if __name__ == "__main__":
   file = open(sys.argv[1])
   postaged_file = open(sys.argv[2])
   for line in file:
      posline = postaged_file.readline()
      line = line.strip()
      posline = posline.strip()
      items = line.split()
      positems = posline.split(); out = []
      for index in range(len(items)):
         word = items[index].split('|')
         pos = positems[index].split('|')
         assert word[0] == pos[0]
         word[1] = pos[1]
         out.append('|'.join(word))
      print " ".join(out)
   file.close(); postaged_file.close()
