# input: lists of words. output: common words count 
def commonwords(sen1, sen2):
   d1 = {}
   d2 = {}
   common = 0

   for w1 in sen1:
      if not w1 in d1:
         d1[w1] = 0
      d1[w1] += 1

   for w2 in sen2:
      if not w2 in d2:
         d2[w2] = 0
      d2[w2] += 1

   for w1 in d1:
      common += min(d1[w1], d2.get(w2, 0))

   return common
