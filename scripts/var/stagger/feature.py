import sys
import getopt
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../../"))

import tools.integerization

def oneFeat(prefix, content, integerizer):
   if integerizer:
      return integerizer.find_or_insert(prefix + ': ' + content)
   return prefix + ': ' + content

def addFeat(retval, feat):
   if not feat in retval:
      retval[feat] = 0
   retval[feat] += 1

def extractFeatures(sent, index, integerizer):
   g_beginTag = '-BEG-'
   retval = {}
   assert index>=0 and index<len(sent)
   token = sent[index][0]
   pos = sent[index][1]
   if index>0:
      tokenl1 = sent[index-1][0]
      posl1 = sent[index-1][1]
   else:
      posl1 = g_beginTag
   if index>1:
      tokenl2 = sent[index-2][0]
      posl2 = sent[index-2][1]
   else:
      posl2 = g_beginTag
   if index>2:
      tokenl3 = sent[index-3][0]
      posl3 = sent[index-3][1]
   else:
      posl3 = g_beginTag
   if index+1 < len(sent):
      tokenr1 = sent[index+1][0]
      posr1 = sent[index+1][1]
   else:
      posr1 = g_beginTag
   if index+2 < len(sent):
      tokenr2 = sent[index+2][0]
      posr2 = sent[index+2][0]
   else:
      posr2 = g_beginTag
   if index+3 < len(sent):
      tokenr3 = sent[index+3][0]
      posr3 = sent[index+3][1]
   else:
      posr3 = g_beginTag
   # decide features
   addFeat(retval, oneFeat("word", token, integerizer))
   addFeat(retval, oneFeat("pos", pos, integerizer))
   if index>0: addFeat(retval, oneFeat("wordl1", tokenl1, integerizer))
   addFeat(retval, oneFeat("posl1", posl1, integerizer))
   if index>1: addFeat(retval, oneFeat("wordl2", tokenl2, integerizer))
   if index>0: addFeat(retval, oneFeat("posl2", posl2, integerizer))
#   if index>2: addFeat(retval, oneFeat("wordl3", tokenl3, integerizer))
#   if index>1: addFeat(retval, oneFeat("posl3", posl3, integerizer))
   if index+1<len(sent): addFeat(retval, oneFeat("wordr1", tokenr1, integerizer))
   addFeat(retval, oneFeat("posr1", posr1, integerizer))
   if index+2<len(sent): addFeat(retval, oneFeat("wordr2", tokenr2, integerizer))
   if index+1<len(sent): addFeat(retval, oneFeat("posr2", posr2, integerizer))
#   if index+3<len(sent): addFeat(retval, oneFeat("wordr3", tokenr3, integerizer))
#   if index+2<len(sent): addFeat(retval, oneFeat("posr3", posr3, integerizer))
   return retval

if __name__ == "__main__":
   opts, args = getopt.getopt(sys.argv[1:], "b:", [])
   sIntegerizerPath = ''
   for opt in opts:
      if opt[0] == "-b":
         sIntegerizerPath = opt[1]

   if len(args) < 1:
      print "usage: extractfeatures.py [-b int_file] input >output"
      print "the input file is a constituent file, which contains one sentence in a line, followed by each constituent. it can be generated from ccg/parsetree2constituents.py, for example"
      sys.exit(1)

   if sIntegerizerPath:
      integerizer = tools.integerization.CIntegerization()
   else:
      integerizer = None

   for feature in extractFeat(args[0], integerizer):
      print feature[0], feature[1:]

   if integerizer:
      integerizer.write(sIntegerizerPath)
