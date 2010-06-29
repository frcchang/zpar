import cPickle

# Integerize from 1 to infinity; 0 means no entry
class CIntegerization(object):

   # bKeys - store key for inverse find
   def __init__(self, bKeys=False):
      self.m_nCurrent = 1
      self.m_dRecord = {}
      if bKeys:
         self.m_lKeys = [None]
      self.m_bKeys = bKeys

   def find(self, oItem):
      return self.m_dRecord.get(oItem, 0)

   def find_or_insert(self, oItem):
      nSeqNumber = self.m_dRecord.get(oItem, 0)
      if nSeqNumber == 0:
         self.m_dRecord[oItem] = self.m_nCurrent
         self.m_nCurrent += 1
         if self.m_bKeys: self.m_lKeys.append(oItem)
         return self.m_nCurrent-1
      return nSeqNumber

   def key(self, nIndex):
      assert self.m_bKeys
      return self.m_lKeys[nIndex]

   def write(self, path):
      file = open(path, 'w')
      cPickle.dump(self, file)
      file.close()

   @classmethod
   def read(cls, path):
      file = open(path)
      self = cPickle.load(file)
      file.close()
      return self

if __name__ == '__main__':
   print "testing..."
   i = CIntegerization()
   i.find_or_insert('a')
   i.find_or_insert('b')
   print i.find('d')
   print i.find('b')
   print i.find_or_insert('c')
   print "did i print 0 2 3?"
