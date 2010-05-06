import sys

if __name__ == "__main__":
   file = open(sys.argv[1])
   g_thresh = float(sys.argv[2])/100
   line = file.readline()
   tokens = []
   while line:
      line = line.strip()
      if not line: print " ".join(tokens); tokens=[]; line=file.readline(); continue
      item = line.split()
      prob = float(item[4])
      if item[3] == '0' and prob < g_thresh or item[3]=='1':
         tokens.append(item[0]+'|'+item[1]+'|1')
      else:
         tokens.append(item[0]+'|'+item[1]+'|0')
      line = file.readline()
   # job done      
   file.close
