def getsent(path):
   file = open(path)
   for line in file:
      yield [word.split('|') for word in line.split()]
   file.close()
