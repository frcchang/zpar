import sys

if __name__ == "__main__":
   file = open(sys.argv[1])
   file_r = open(sys.argv[2])
   total = 0
   correct = 0
   list = []
   for line in file:
      line = line.strip()
      if not line: 
         line_r = file_r.readline()
         line_r = line_r.split()
         if len(list) != len(line_r):
            print list, len(list)
            print line_r
         assert len(list) == len(line_r)
         for index in range(len(list)):
            total += 1
            wr = line_r[index].split('|')[2]
            if wr in list[index]:
               correct += 1
         list = []
         continue
      list.append([])
      line = line.split()
      for i in range(int(line[2])):
         list[-1].append(line[3+i*2])

   file_r.close()
   file.close()
   print float(correct) / total
