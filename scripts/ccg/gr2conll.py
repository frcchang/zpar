import sys

def readbinaryrules(path):
   d = {}
   file = open(path)
   for line in file:
      line = line.strip().split()
      assert line[1] == ','
      assert line[3] == ':'
      d[(line[0], line[2])] = []
      last_index = 4
      for index in range(5, len(line)):
         if line[index] == ']':
            d[(line[0], line[2])].append(' '.join(line[last_index+1:index]))
         elif line[index] == ',':
            d[(line[0], line[2])].append(' '.join(line[last_index+1:index]))
            assert line[index+1] == 'REDUCE'
            last_index = index
   file.close()
   return d

def readunaryrules(path):
   d = {}
   file = open(path)
   for line in file:
      line = line.strip().split()
      assert line[1] == ':'
      d[line[0]] = []
      last_index = 2
      assert line[2] == '['
      for index in range(3, len(line)):
         if line[index] == ']':
            d[line[0]].append(' '.join(line[last_index+1:index]))
         elif line[index] == ',':
            d[line[0]].append(' '.join(line[last_index+1:index]))
            assert line[index+1] == 'REDUCE'
            last_index = index
   file.close()
   return d

def printbinaryrule(d):
   for key in d:
      if d[key]:
         print '%s , %s : [ %s ]' % (key[0], key[1], ' , '.join(d[key]))

def printunaryrule(d):
   for key in d:
      if d[key]:
         print '%s : [ %s ]' % (key, ' , '.join(d[key]))

def printbinaryruleexpanded(d):
   for key in d:
      for action in d[key]:
         print key[0], key[1], action.split()[-1]

def printunaryruleexpanded(d):
   for key in d:
      for action in d[key]:
         print key, action.split()[-1]

if __name__ == '__main__':
   if len(sys.argv) < 2:
      print 'rule.py command'
      sys.exit(1)
   command = sys.argv[1]
   if command == 'subtractbinary':
      d = readbinaryrules(sys.argv[2])
      s = readbinaryrules(sys.argv[3])
      for key in d:
         for item in s.get(key, []):
            d[key].remove(item)
      printbinaryruleexpanded(d)
   elif command == 'subtractunary':
      d = readunaryrules(sys.argv[2])
      s = readunaryrules(sys.argv[3])
      for key in d:
         for item in s.get(key, []):
            d[key].remove(item)
      printunaryruleexpanded(d)
   elif command == 'countbinary':
      d = readbinaryrules(sys.argv[2])
      total = 0
      for key in d:
         total += len(d[key])
      print 'Total rules:', total
   elif command == 'countunary':
      d = readunaryrules(sys.argv[2])
      total = 0
      for key in d:
         total += len(d[key])
      print 'Total rules:', total
