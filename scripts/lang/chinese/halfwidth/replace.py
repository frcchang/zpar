class CReplace(object):

   def __init__(self, path):
      self.ent = {}
      f = open(path)
      for line in f:
         line = line[:-1].split()
         assert len(line) == 2
         assert not line[0] in self.ent
         k = line[0] #unicode(line[0], 'utf-8')
         v = line[1] #unicode(line[1], 'utf-8')
         self.ent[k] = v
      f.close()

   def replace(self, s):
      t = []
      for c in unicode(s, 'utf-8'):
         c = c.encode('utf-8')
         t.append(self.ent.get(c, c))
      return ''.join(t)
