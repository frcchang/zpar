import sys
import posio
import getopt

def builddic(path, d, w):
   file = open(path)
   for line in file:
      tup = tuple(line.split())
      assert len(tup) == 2
      d[tup] = 1
      w[tup[0]] = 1
   file.close()

# pos1 - system, pos2 - reference, pos3 - system2
def eval(pos1, pos2, pos3, d, w, v):
   total = 0
   correct = 0
   oov = 0
   assert len(pos1)==len(pos2)
   assert not pos3 or len(pos3)==len(pos1)
   for index, word in enumerate(pos1):
      word2 = pos2[index]
      if pos3: 
         third = pos3[index]
      total += 1
      assert word[0] == word2[0]
      assert not pos3 or word[0] == third[0]
      if word[1] == word2[1]:
         correct += 1
      else:
         if v:
            if not pos3 or pos3 and third[1] == word2[1]:
               print '%s	%s	%s'%(word[0], word2[1], word[1])
         if (word[0] in w) and (not (word2[0], word2[1]) in d):
            oov += 1
   return total, correct, oov

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], 'vt:', {})
   ref_file = args[1]
   out_file = args[0]
   verbose = False
   third = None
   for opt in opts:
      if opt[0] == '-v':
         verbose = True
      if opt[0] == '-t':
         third = opt[1]
         verbose = True
   if verbose:
   #if v:
      print 'Word	correct	system'
      print '='*30
   dic = {}
   words = {}
   if len(args) == 3:
      builddic(args[2], dic, words)
   ref = posio.posread(ref_file)
   out = posio.posread(out_file)
   if third:
      third = posio.posread(third)
   total = 0
   correct = 0
   oov = 0
   sent_third = None
   for sent in out:
      sent_ref = ref.next()
      if third: 
         sent_third = third.next()
      res = eval(sent, sent_ref, sent_third, dic, words, verbose)
      total += res[0]
      correct += res[1]
      oov += res[2]
   print 'precision:', float(correct) / total
   if dic:
      print 'out dic:', float(oov) / total
