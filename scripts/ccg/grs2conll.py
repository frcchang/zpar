import sys
import getopt
import grsio

def grs2conll(sent):
   words = sent[0]
   links = sent[1]
   retval = []
   for index, word in enumerate(words):
      retval.append([str(index+1), word[0], '_', word[1], word[1], word[2], "0", "_", '_', '_'])
   for link in links:
      rel = link[0]
      head = link[1]
      mod = link[2]
      rword = retval[mod-1]
      if rword[6] == "0":
         rword[6] = str(head)
      else:
         rword[6] += ','
         rword[6] += str(head)
      if rword[7] == '_':
         rword[7] = rel
      else:
         rword[7] += ','
         rword[7] += rel
   return retval
      
if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], '', {})
   except:
      print 'grs2conll.py input >output'
      sys.exit(1)
   for sent in grsio.readSent(args[0]):
      cons = grs2conll(sent)
      for word in cons:
         print '\t'.join(word)
      print
