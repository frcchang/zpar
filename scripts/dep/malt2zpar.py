import depio

def malt2zpar(path):
   for sent in depio.depread(path):
      for word in sent:
         word[2] = str(int(word[2])-1)
      depio.depprint(sent)

if __name__ == '__main__':
   import sys
   malt2zpar(sys.argv[1])
