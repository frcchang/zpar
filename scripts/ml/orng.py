import sys
import os
import orange

class COrangeWriter(object):

   def __init__(self, path, bTrain):
      self.bTrain = bTrain
      if bTrain:
         self.file = open(path+'.tab', "w")
      else:
         data = orange.ExampleTable(path)
         self.file = orange.C45Learner(data, prune=False)
      self.names = None
      self.types = None
      self.classes = None
      self.examples = []

   #def __del__(self):
   #   self.file.close()
   #
   def setFeatureDefs(self, names, types, classes):
      if not self.names:
         assert not self.types
         assert not self.classes
         self.names = names
         self.types = types
         self.classes = classes

   def addExample(self, example):
      assert self.bTrain
      self.examples.append(example)

   def predict(self, example):
      assert not self.bTrain
      return self.file(orange.Example(self.file.domain, example))

   def train(self):
      assert self.bTrain
      if self.names:
         assert self.types
         assert self.classes
         self.file.write("\t".join(self.names))
         self.file.write("\n")
         self.file.write("\t".join(self.types))
         self.file.write("\n")
         self.file.write("\t".join(self.classes))
         self.file.write("\n")
      for example in self.examples:
         self.file.write("\t".join(example))
         self.file.write("\n")

#================================================================================

if __name__ == '__main__':
   input = open(sys.argv[1])
   model = COrangeWriter(sys.argv[2], False)
   for line in input:
      line = line[:-1]
      data = line.split('\t')
      data[-1] = str(model.predict(data))
      print '\t'.join(data)
   input.close()
