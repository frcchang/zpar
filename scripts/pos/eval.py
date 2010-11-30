import sys
import posio

def builddic(path, d, w):
   file = open(path)
   for line in file:
      tup = tuple(line.split())
      assert len(tup) == 2
      d[tup] = 1
      w[tup[0]] = 1
   file.close()

def eval(pos1, pos2, d, w):
   total = 0
   correct = 0
   oov = 0
   for index, word in enumerate(pos1):
      word2 = pos2[index]
      total += 1
      assert word[0] == word2[0]
      if word[1] == word2[1]:
         correct += 1
      else:
         if (word[0] in w) and (not (word2[0], word2[1]) in d):
            oov += 1
   return total, correct, oov

if __name__ == '__main__':
   ref_file = sys.argv[2]
   out_file = sys.argv[1]
   dic = {}
   words = {}
   if len(sys.argv) == 4:
      builddic(sys.argv[3], dic, words)
   ref = posio.posread(ref_file)
   out = posio.posread(out_file)
   total = 0
   correct = 0
   oov = 0
   for sent in out:
      sent_ref = ref.next()
      res = eval(sent, sent_ref, dic, words)
      total += res[0]
      correct += res[1]
      oov += res[2]
   print 'precision:', float(correct) / total
   if dic:
      print 'out dic:', float(oov) / total
