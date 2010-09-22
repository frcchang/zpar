import sys
import depio

def labelanal(sent, dLabel, setPOS):
   for word in sent:
      head = int(word[2])
      pos = word[1]
      label = word[3]
      setPOS.add(pos)
      if head == -1:
         assert label == 'ROOT'
      else:
         head_pos = sent[head][1]
         setPOS.add(head_pos)
         if not label in dLabel:
            dLabel[label] = {}
         if not head_pos in dLabel[label]:
            dLabel[label][head_pos] = {}
         if not pos in dLabel[label][head_pos]:
            dLabel[label][head_pos][pos] = 0
         dLabel[label][head_pos][pos] += 1

if __name__ == "__main__":
   dLabel = {}
   setPOS = set([])
   for sent in depio.depread(sys.argv[1]):
      labelanal(sent, dLabel, setPOS)
   for label in dLabel:
      print label, '==='
      setDeps = set([])
      dPair = dLabel.get(label, {})
      for head in dPair:
         headCount = 0
         dDep = dPair.get(head, {})
         for key in dDep.keys():
            headCount += dDep[key]
         print head, '(', headCount, ')', ' : ', ' '.join([key+'('+str(dDep[key])+')' for key in dDep.keys()])
         setDeps = setDeps.union(dDep.keys())
      print
      print 'Set of heads: ', ' '.join(dPair.keys())
      print 'Set of deps: ', ' '.join(setDeps)
      print
      print 'Set of nonheads: ', ' '.join(setPOS-set(dPair.keys()))
      print 'Set of nondeps: ', ' '.join(setPOS-setDeps)
      print
