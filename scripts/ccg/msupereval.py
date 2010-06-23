import sys

if __name__ == "__main__":
   file = open(sys.argv[1]) # output
   file_r = open(sys.argv[2]) # reference
   total = 0
   correct = 0
   count = 0
   error = 0
   list = []
   sentence_index=1
   missing = []
   for line in file:
      line = line.strip()
      if not line: 
         line_r = file_r.readline()
         line_r = line_r.split()
         if len(list) != len(line_r):
            print list, len(list)
            print line_r
         assert len(list) == len(line_r)
         count += 1
         wrong = 0
         for index in range(len(list)):
            total += 1
            wr = line_r[index].split('|')[2]
            if wr in list[index]:
               correct += 1
            else:
               wrong = 1
         if wrong == 1:
            missing.append(sentence_index)
         error += wrong
         list = []
         sentence_index+=1
         continue
      list.append([])
      line = line.split()
      for i in range(int(line[2])):
         list[-1].append(line[3+i*2])

   file_r.close()
   file.close()
   print float(correct) / total, "sentence ratio ", float(error) / count
   print len(missing), 'sentences miss the correct category:', missing
