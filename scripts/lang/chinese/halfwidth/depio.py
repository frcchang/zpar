import sys

def depread(path):
   """read dependency trees"""
   file = open(path)
   sent = []
   for line in file:
      if not line.strip():
         yield sent
         sent = []
      else:
         line = line[:-1].split('\t')
         sent.append(line)
   file.close()

   if sent: 
      yield sent

def depstring(sent):
   retval = ''
   for line in sent:
      retval += "\t".join(line)
      retval += '\n'
   retval += '\n'
   return retval

def depprint(sent):
   for line in sent:
      print "\t".join(line)
   print

if __name__ == "__main__":
   for sent in depread(sys.argv[1]):
      depprint(sent)
