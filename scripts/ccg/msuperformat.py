import sys

if __name__ == "__main__":
   file = open(sys.argv[1])
   for line in file:
      line = line.strip()
      if line:
         line = line.split()
         output = []
         output.append(line[0])
         output.append(line[1])
         for index in range(int(line[2])):
            output.append(line[2+2*index+1])
         print " ".join(output)
      else:
         print
