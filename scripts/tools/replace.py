class CReplace(object):

   def __init__(self, path):
      self.ent = {}
      f = open(path)
      for line in f:
         line = line[:-1].split()
         assert len(line) == 2
         assert not line[0] in self.ent
         self.ent[line[0]] = line[1]
      f.close()

   def replace(self, s):
      t = []
      for c in s:
         t.append(self.ent.get(c, c))
      return ''.join(t)
