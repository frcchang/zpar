import sys
import getopt
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../../"))

import constituentreader
import tools.integerization

def oneFeat(prefix, content, integerizer):
   if integerizer:
      return integerizer.find_or_insert(prefix + ': ' + content)
   return prefix + ': ' + content

def addFeat(retval, feat):
   if not feat in retval:
      retval[feat] = 0
   retval[feat] += 1

def extractOneFeat(sent, constituent, integerizer):
   retval = {}
   name = constituent[2]
   start = int(constituent[0])
   end = int(constituent[1])
   size = end-start+1
   # decide label
   if name == None:
      class_label = -1
   elif name == 'NP':
      class_label = 1
   else:
      class_label = 0
   # decide features
#   retval.append(oneFeat("word_0", sent[start][0], integerizer))
#   retval.append(oneFeat("word_n", sent[end][0], integerizer))
#   if start > 0:
#      retval.append(oneFeat("word_0 word_{-1}", sent[start][0] + ' ' + sent[start-1][0], integerizer))
#   if end < len(sent)-1:
#      retval.append(oneFeat("word_n word_{n+1}", sent[end][0] + ' ' + sent[end+1][0], integerizer))
#   retval.append(oneFeat("word_0 word_n", sent[start][0] + ' ' + sent[end][0], integerizer))
#   retval.append(oneFeat("pos_0", sent[start][1], integerizer))
#   retval.append(oneFeat("pos_n", sent[end][1], integerizer))
#   pos2 = '-BEGIN-'
#   if start>1: pos2 = sent[start-2][1]
#   pos1 = '-BEGIN-'
#   if start>0: pos1 = sent[start-1][1]
#   retval.append(oneFeat("pos_{-1} pos_0", pos1 + ' ' + sent[start][1], integerizer))
#   retval.append(oneFeat("pos_{-2} pos_{-1} pos_0", pos2 + ' ' + pos1 + ' ' + sent[start][1], integerizer))
#   if size>2:
#      retval.append(oneFeat("pos_0 pos_1", sent[start][1] + ' ' + sent[start+1][1], integerizer))
#      retval.append(oneFeat("pos_{-1} pos_0 pos_1", pos1 + ' ' + sent[start][1] + ' ' + sent[start+1][1], integerizer))
#   pos2 = '-BEGIN-'
#   if end<len(sent)-2: pos2 = sent[end+2][1]
#   pos1 = '-BEGIN-'
#   if end<len(sent)-1: pos1 = sent[end+1][1]
#   retval.append(oneFeat("pos_n pos_{n+1}", sent[end][1] + ' ' + pos1, integerizer))
#   retval.append(oneFeat("pos_n pos_{n+1} pos_{n+2}", sent[end][1] + ' ' + pos1 + ' ' + pos2, integerizer))
#   if size>2:
#      retval.append(oneFeat("pos_{n-1} pos_n", sent[end-1][1] + ' ' + sent[end][1], integerizer))
#      retval.append(oneFeat("pos_{n-1} pos_n pos_{n+1}", sent[end-1][1] + ' ' + sent[end][1] + ' ' + pos1, integerizer))
#   if size>40:
#      size=8
#   elif size>20:
#      size=7
#   elif size>10:
#      size=6
#   elif size>5:
#      size=5
#   retval.append(oneFeat("word_0 size", sent[start][0] + ' ' + str(size), integerizer))
#   retval.append(oneFeat("word_n size", sent[end][0] + ' ' + str(size), integerizer))
#   for index in range(start, end+1):
#      retval.append(oneFeat("word size", sent[index][0] + ' ' + str(size), integerizer))
#      retval.append(oneFeat("pos size", sent[index][1] + ' ' + str(size), integerizer))
#   for index in range(start, end):
#      retval.append(oneFeat("pos bigram", sent[index][1] + ' ' + sent[index+1][1], integerizer))
#      retval.append(oneFeat("pos bigram size", sent[index][1] + ' ' + sent[index+1][1] + ' ' + str(size), integerizer))
#   for index in range(start, end-1):
#      retval.append(oneFeat("pos trigram", sent[index][1] + ' ' + sent[index+1][1] + ' ' + sent[index+2][1], integerizer))
   setpos = set()
   for index in range(start, end+1):
      setpos.add(sent[index][1])
   addFeat(retval, oneFeat("pos set", repr(setpos), integerizer))
   return (class_label, retval)

def extractFeat(path, integerizer):
   for sent, constituents in constituentreader.read(path):
      for constituent in constituents:
         yield extractOneFeat(sent, constituent, integerizer)

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
