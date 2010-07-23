import ctypes
import io
import feature
from libsvm import svm, svmutil
import sys

import getopt

import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))


import tools.integerization

cutoff = 0.99

def predict(data, path):
   if not os.path.exists(path):
      print 'Model path does not exist, do nothing.'
      return
   print "Loading features."
   integerizer = tools.integerization.CIntegerization.read(os.path.join(path, 'int'))
   model = svmutil.svm_load_model(os.path.join(path, "scr"))  
   print "Working."
   for sent in io.getsent(data):
      labels = []
      for index in range(len(sent)):
         f = feature.extractFeatures(sent, index, integerizer)
         nodes, x = svmutil.gen_svm_nodearray(f)
         probabilities = (ctypes.c_double*2)()
         label = svm.libsvm.svm_predict_probability(model, nodes, probabilities)
         x = 1
         if probabilities[0] > cutoff:
            x = 0
         labels.append(str(x))
      print ' '.join(labels)

if __name__ == "__main__":
   opts, args = getopt.getopt(sys.argv[1:], "", [])

   if len(args) < 2:
      print "usage: predict.py input model"
      sys.exit(1)

   predict(args[0], args[1])
