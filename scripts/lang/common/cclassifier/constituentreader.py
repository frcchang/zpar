import sys

def read(path):
   file = open(path)
   bNewSentence = True
   lSentence = []
   lConstituents = []
   for line in file:
      if not line.strip():
         bNewSentence = True
         yield (lSentence, lConstituents)
         lConstituents = []
         continue
      if bNewSentence:
         lSentence = [word.split('|') for word in line.split()]
         bNewSentence = False
      else:
         lConstituents.append(line.split())
   file.close()
      
if __name__ == "__main__":
   for sent, constituents in read(sys.argv[1]):
      print sent
      for constituent in constituents:
         print constituent
      print
