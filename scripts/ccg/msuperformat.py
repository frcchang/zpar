import sys

def insertprobabilities(path):
   file = open(path)
   for line in file:
      line = line.strip()
      if line:
         line = line.split()
         output = []
         output.append(line[0])
         output.append(line[1])
         output.append(str(len(line)-2))
         for index in range(2, len(line)):
            output.append(line[index])
            output.append('0')
         print ' '.join(output)
      else:
         print
   file.close()

def removeprobabilities(path):
   file = open(path)
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

if __name__ == "__main__":
   insertprobabilities(sys.argv[1])
