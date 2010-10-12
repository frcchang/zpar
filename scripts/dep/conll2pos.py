import sys

def getIdx(s):
   return str(int(s)-1)

if __name__ == "__main__":
   if len(sys.argv) != 2:
      print "conll2dep input_file > output_file"
      sys.exit(1)
   sent = []
   file = open(sys.argv[1])
   for line in file:
      line = line.strip()
      if not line:
         print ' '.join(sent)
         sent = []
         continue
      line = line.split("\t")
      sent.append('/'.join([line[1], line[4]]))
   file.close()

