import extractfeat
from libsvm import *
import sys

import getopt

import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))


import tools.integerization

def train(data, path):
   print "Loading features."
   integerizer = tools.integerization.CIntegerization()
   labels = []
   samples = []
   for classid, feature in extractfeat.extractFeat(data, integerizer):
      labels.append(classid)
      samples.append(feature)
   print "Training SVM."
   problem = svm_problem(labels, samples)
   param = svm_parameter(kernel_type = POLY, degree=2, C=1, eps=1, shrinking=0)
   model = svm_model(problem, param)
   print "Saving model."
   os.mkdir(path)
   model.save(os.path.join(path, "scr"))  
   integerizer.write(os.path.join(path, "int"))

if __name__ == "__main__":
   opts, args = getopt.getopt(sys.argv[1:], "", [])

   if len(args) < 2:
      print "usage: train.py input model"
      sys.exit(1)

   train(args[0], args[1])
