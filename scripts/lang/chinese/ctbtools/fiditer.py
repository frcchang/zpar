#****************************************************************
#
# fiditer.py - iterate through a directory of fid files
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.03
#
#****************************************************************

import sys
import os
import re

#================================================================

def file_iterator(directory, range, file=r"chtb_%s.fid"):
   for tRange in range:
      if type(tRange) == int: 
         tRange = (tRange, tRange)
      for nIndex in xrange(tRange[0], tRange[1]+1):
         sIndex = str(nIndex)
         if nIndex < 1000:
            sIndex = sIndex.rjust(3, '0')
         yield os.path.join(directory, file % sIndex)

#================================================================

g_id_start = re.compile(r'<S ID=[0-9]+>')
g_id_end = re.compile(r'</S>')

def sentence_iterator(process, directory, range, file=r"chtb_%s.fid"):
   for sFile in file_iterator(directory, range):
      try:
         oFile = open(sFile)
      except IOError:
         continue
      sLine = oFile.readline()               
      while sLine:
         if g_id_start.match(sLine):
            sSentence = ""
            sLine = oFile.readline()
            while sLine:
               if g_id_end.match(sLine):
                  break
               sSentence += " " + sLine.strip()
               sLine = oFile.readline()
            else:
               raise "<S> without </S>"
            process(sSentence)
         sLine = oFile.readline()              
      oFile.close()

#================================================================

if __name__ == '__main__':
   import config
   cf = config.CConfig("fid.config")
   def printx(x):
      print x
   sentence_iterator( printx, cf.directory, cf.range )
