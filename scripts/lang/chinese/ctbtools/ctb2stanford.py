#****************************************************************
#
# ctb2stanford.py - reformat POS tags from ctb format to stanford-tagger format.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2006.11
#
#****************************************************************

#================================================================
#
# Import modules assuming that libs has been appended to sys.path
#
#================================================================

import sys

#================================================================
#
# main
#
#================================================================

if __name__ == '__main__':
   if len(sys.argv) != 3:
      print "Usage: %s input output" % sys.argv[0]
      sys.exit(1)
   bIgnoreNoneTag = True
   oFile = open(sys.argv[1])
   outputFile = open(sys.argv[2], "w")
   # 1. read one line
   sLine = "\n"                                 # use a pseudo \n to start
   while sLine:                                 # while there is a line
      sLine = sLine.strip()                     # strip the line
      if not sLine:                             # if the line isn't empty
         sLine = oFile.readline()               # break
         continue
      # 2. analyse this line
      lLine = sLine.strip().split(" ")
      lNewLine = []
      for nIndex in xrange(len(lLine)):
         tTagged = lLine[nIndex].split("_")
         assert len(tTagged)==2
         tTagged[0].replace("/", "//")
         if (bIgnoreNoneTag==False) or (tTagged[0]): # if we take -NONE- tag, or if we find that the tag is not -NONE-
            lNewLine.append("/".join(tTagged))
      outputFile.write(" ".join(lNewLine))
      outputFile.write("\n")
      sLine = oFile.readline()

   oFile.close()
   outputFile.close()
