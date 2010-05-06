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
      values = {}
      keys = []
      count = int(item[2])
      for i in range(count):
         values[item[3+2*i]] = float(item[3+2*i+1])
         keys.append(item[3+2*i])
      keys.sort()
      prob = 0
      while prob < g_thresh:
         if not keys:
            value = '0'
            break
         if keys[0] != '0':
            prob += values[keys[0]]
            value = keys[0]
         keys.pop(0)
      tokens.append('|'.join([item[0], item[1], value]))
      line = file.readline()
   # job done      
   file.close
