import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'pos'))
import depio
import posio

def mergeconllpos(conll_path, pos_path):
   pos = posio.posread(pos_path)
   for sent in depio.depread(conll_path):
      sentpos = pos.next()
      assert len(sent) == len(sentpos)
      for n in range(len(sent)):
#         print sent[n][1], sentpos[n][0]
         assert sent[n][1] == sentpos[n][0]
         sent[n][4] =sentpos[n][1]
      depio.depprint(sent)

if __name__ == '__main__':
   mergeconllpos(sys.argv[1], sys.argv[2])
