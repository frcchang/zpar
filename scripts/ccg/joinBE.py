import sys

if __name__ == "__main__":
   file = open(sys.argv[1])
   line = file.readline()
   while line:
      token = line.split()
      o = []
      for index in range(len(token)):
         elem =token[index].split('|')
         assert len(elem) == 4
         o.append('|'.join([elem[0], elem[1], elem[2]+elem[3]]))
      print ' '.join(o)
      line = file.readline()
   file.close()
