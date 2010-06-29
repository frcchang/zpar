import extractfeat
from libsvm import svm, svmutil
import sys

import getopt

import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))


import tools.integerization

def train(data, path):
   if os.path.exists(path):
      print 'Model path exists, do nothing.'
      return
   print "Loading features."
   integerizer = tools.integerization.CIntegerization()
   labels = []
   samples = []
   for classid, feature in extractfeat.extractFeat(data, integerizer):
      labels.append(classid)
      samples.append(feature)
   print "Training SVM."
   problem = svm.svm_problem(labels, samples)
   param = svm.svm_parameter()
   param.kernel_type = svm.LINEAR#
   param.C=1
   #param.degree=2
   param.eps=1
   param.probability=1
   model = svmutil.svm_train(problem, param)
   print "Saving model."
   os.mkdir(path)
   svmutil.svm_save_model(os.path.join(path, "scr"), model)  
   integerizer.write(os.path.join(path, "int"))

if __name__ == "__main__":
   opts, args = getopt.getopt(sys.argv[1:], "", [])

   if len(args) < 2:
      print "usage: train.py input model"
      sys.exit(1)

   train(args[0], args[1])
