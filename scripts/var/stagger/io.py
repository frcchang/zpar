tag_separator = '_'

def getsent(path):
   file = open(path)
   for line in file:
      yield [word.split(tag_separator) for word in line.split()]
   file.close()
