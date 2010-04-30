import sys

def depread(path):
   file = open(path)
   sent = []
   for line in file:
      if not line.strip():
         yield sent
         sent = []
      else:
         line = line.split()
         sent.append(line)
   file.close()

def depprint(sent):
   for line in sent:
      print "\t".join(line)
   print

if __name__ == "__main__":
   for sent in depread(sys.argv[1]):
      depprint(sent)
