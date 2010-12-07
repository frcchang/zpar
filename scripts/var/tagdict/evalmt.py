import sys

import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../pos'))

import posio

def evalmt(pos1, pos2):
   correct = 0
   for word1 in pos1: 
      match = -1
      for i2 in range(len(pos2)):
         if pos2[i2][0] == word1[0] and pos2[i2][1] == word1[1]:
            correct+=1
            match = i2
            break
      if match != -1:
         pos2[match] = [None, None]
   return correct, len(pos1)

if __name__ == '__main__':
   ref_file = posio.posread(sys.argv[2])
   out_file = posio.posread(sys.argv[1])
   total = 0
   correct = 0
   for sent in out_file:
      ref = ref_file.next()
      retval = evalmt(sent, ref)
      total += retval[1]
      correct += retval[0]
   print 'precision: ', float(correct) / total
