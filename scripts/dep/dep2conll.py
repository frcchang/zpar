import sys
import depio

def conll(dep):
   index = 1
   for word in dep:
      if len(word) == 4:
         label = word[3]
      else:
         label = '_'
      print '\t'.join([str(index), word[0], '_', word[1], word[1], '_', str(int(word[2])+1), label, '_', '_'])
      index += 1
   print

if __name__ == "__main__":
   if len(sys.argv) != 2:
      print "conll2dep input_file > output_file"
      sys.exit(1)#return
   for sent in depio.depread(sys.argv[1]):
      conll(sent)
