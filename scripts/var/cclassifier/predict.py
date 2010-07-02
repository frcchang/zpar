import extractfeat
from libsvm import svm, svmutil
import sys

import getopt
import posreader

import os

import ctypes

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))


import tools.integerization

def predict(data, path):
   if not os.path.exists(path):
      print 'Model path does not exist, do nothing.'
      return
   print "Loading features."
   integerizer = tools.integerization.CIntegerization.read(os.path.join(path, 'int'))
   model = svmutil.svm_load_model(os.path.join(path, "scr"))  
   print "Decoding."
   for sent in posreader.readpossent(data):
      print sent
      for start in range(len(sent)):
         for end in range(start, len(sent)):
            label, feature = extractfeat.extractOneFeat(sent, (start, end, None), integerizer)
            assert label == -1
            nodes, x = svmutil.gen_svm_nodearray(feature)
            label = svm.libsvm.svm_predict(model, nodes)
            probabilities = (ctypes.c_double*2)()
            label = svm.libsvm.svm_predict_probability(model, nodes, probabilities)
            print start, end, probabilities[0], probabilities[1]

if __name__ == "__main__":
   opts, args = getopt.getopt(sys.argv[1:], "", [])

   if len(args) < 2:
      print "usage: predict.py input model"
      sys.exit(1)

   predict(args[0], args[1])
