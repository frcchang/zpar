import sys

if __name__ == "__main__":
   refCount = 0
   outCount = 0
   match = 0
   refFile = open(sys.argv[1])
   outFile = open(sys.argv[2])
   refLine = refFile.readline()
   outLine = outFile.readline()
   while refLine:
      assert outLine
      ref = refLine.split(); out = outLine.split()
      for index in range(len(ref)):
         refToken = ref[index]
         outToken = out[index]
         refToken = refToken.split('|')
         outToken = outToken.split('|')
         assert refToken[0] == outToken[0]
         #assert refToken[1] == outToken[1]
         if refToken[2] == '0':
            refCount += 1
         if outToken[2] == '0':
            outCount += 1
         if refToken[2] == '0' and outToken[2] == '0':
            match += 1
         index += 1
      refLine = refFile.readline()
      outLine = outFile.readline()
   p = float(match)/outCount
   r = float(match)/refCount
   f = 2*p*r / (p+r)
   print "precision:", p
   print "recall:", r
   print "F-score:", f
   refFile.close()
   outFile.close()
