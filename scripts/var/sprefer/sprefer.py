import fileinput
import linecache

cutoff = 0.000001 

def findindex(path):
   h = {}
   d = {}
   j = 0
   for line in fileinput.input(path):
      if j == 0:
         k = 0
         for word in line.split():
            h[word] = k
            k += 1
      elif j == 1:
         k = 0
         for word in line.split():
            assert word == 'OOV' or word[-4:] == 'edge'
            if word != 'OOV': word = word[:-4]
            d[word] = k
            k += 1
      else:
         break
      j += 1
   return h, d
      
def findline(path, index):
   return [float(x) for x in linecache.getline(path, index).split()]

def readsent(inpt):
   file = open(inpt)
   for line in file: yield [word.split('/') for word in line.split()]
   file.close()

def sprefer(data, pref):
   h, d = findindex(pref)
   for sent in readsent(data):
      dindex = []
      hindex = []
      result = []
      for word in sent:
         dindex.append(d.get(word[0], d['OOV']))
         hindex.append(h.get(word[0], h['OOV']))
#      print dindex
#      print hindex
      for j in range(len(sent)): # for dep
         column = [] # the row result
         maxpro = None 
         pro = [] # head
         line = findline(pref, dindex[j]+3) # dep probs
         assert line != []
         for i in range(len(sent)): # for head
            pro.append(line[hindex[i]]) # find head probability
            if maxpro == None or pro[-1] >maxpro:
               maxpro = pro[-1]
         for i in range(len(sent)):
            if i != j and pro[i] > maxpro * cutoff:
               column.append(1)
            else:
               column.append(0)
         result.append(column)
      output = []
      for i in range(len(sent)):
         for j in range(len(sent)):
            output.append(str(result[j][i]))
      print ' '.join(output)

if __name__ == '__main__':
   import sys
   sprefer(sys.argv[1], sys.argv[2])
