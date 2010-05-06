import sys

if __name__ == "__main__":
   count_be = 0
   match_be = 0
   count_en = 0
   match_en = 0
   stats_be = {}
   stats_en = {}
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
         if refToken[2] == outToken[2]:
            match_be += 1
         if refToken[3] == outToken[3]:
            match_en += 1
         count_be += 1
         count_en += 1
         if not outToken[2] in stats_be:
            stats_be[outToken[2]] = [0,0]
         if not refToken[2] in stats_be:
            stats_be[refToken[2]] = [0,0]
         stats_be[outToken[2]][0] += 1
         stats_be[refToken[2]][1] += 1
         if not outToken[3] in stats_en:
            stats_en[outToken[3]] = [0,0]
         if not refToken[3] in stats_en:
            stats_en[refToken[3]] = [0,0]
         stats_en[outToken[3]][0] +=1
         stats_en[refToken[3]][1] +=1
      refLine = refFile.readline()
      outLine = outFile.readline()
   p_be = float(match_be)/count_be
   p_en = float(match_en)/count_en
   assert count_be == count_en
   print "begin tag precision:", p_be
   print "end tag precision:", p_en
   print
   print "begin statistics"
   l = stats_be.keys()
   l.sort()
   for k in l:
      print k ,":", stats_be[k][0], stats_be[k][1]
   print
   print "end statistics"
   l = stats_en.keys()
   l.sort()
   for k in l:
      print k, ":", stats_en[k][0], stats_en[k][1]
   refFile.close()
   outFile.close()
