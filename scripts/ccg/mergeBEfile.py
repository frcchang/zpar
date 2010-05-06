import sys

if __name__ == "__main__":
   beginFile = open(sys.argv[1])
   endFile = open(sys.argv[2])
   beginLine = beginFile.readline()
   endLine = endFile.readline()
   while beginLine:
      assert endLine
      begin = beginLine.strip().split()
      end = endLine.strip().split()
      output = []
      for index in range(len(begin)):
         begins = begin[index].split("|")
         ends = end[index].split("|")
         assert begins[0] == ends[0] and begins[1] == ends[1]
         output.append("|".join((begins[0], begins[1], begins[2], ends[2])))
      print " ".join(output)
      beginLine = beginFile.readline()
      endLine = endFile.readline()
   beginFile.close()
   endFile.close()
