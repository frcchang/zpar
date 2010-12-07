import re
import sys
import gzip

reprec = re.compile('precision: ([0-9]+\.[0-9]+)')
reoov = re.compile('out dic: ([0-9]+\.[0-9]+)')

def findmax(path):
   if path.endswith(".gz"):
      file = gzip.open(path)
   else:
      file = open(path)
   idx=0; values=[]; maxidx=0
   line = file.readline()
   while line:
      pat = reprec.match(line)
      while not pat:
         line = file.readline()
         if not line:
            return 1+maxidx, values[maxidx]
         pat = reprec.match(line)
      dev_prec = float(pat.group(1))

      line = file.readline()
      assert line
      pat = reoov.match(line)
      if pat:
         dev_oov = float(pat.group(1))
         line = file.readline()
      else:
         dev_oov = '-'

      pat = reprec.match(line)
      while not pat:
         line = file.readline()
         assert line
         pat = reprec.match(line)
      test_prec = float(pat.group(1))

      line = file.readline()
      if not line:
         return 1+maxidx, values[maxidx]
      pat = reoov.match(line)
      if pat:
         test_oov = float(pat.group(1))
         line = file.readline()
      else:
         test_oov = '-'

#      line = file.readline()
      if not line:
         return maxidx, values[maxidx]
      if line.startswith("Training"):
         brown_prec = '-'
         brown_oov = '-'
      else:
         assert line.startswith('Tagging')
         pat = reprec.match(line)
         while not pat:
            line = file.readline()
            assert line
            pat = reprec.match(line)
         brown_prec = float(pat.group(1))
   
         line = file.readline()
         if not line:
            return 1+maxidx, values[maxidx]
         pat = reoov.match(line)
         if pat:
            brown_oov = float(pat.group(1))
         else:
            brown_oov = '-'

         

      values.append([dev_prec, dev_oov, test_prec, test_oov, brown_prec, brown_oov])
      if values[idx][0] > values[maxidx][0]:
         maxidx = idx
      idx += 1
   file.close()

if __name__ == '__main__':
   retval = findmax(sys.argv[1])
   
   print retval
