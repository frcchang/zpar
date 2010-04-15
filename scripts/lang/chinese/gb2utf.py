# 
# gb2utf - switch encoding between text
#
# Yue Zhang 2006
#
g_sWelcome = """
gb2utf - switch encoding between text

Usage:  gb2utf input output

Author: Yue Zhang, 2006
"""
import sys

#
# Main entry
#
iFile = open(sys.argv[1])
oFile = open(sys.argv[2], "w")
sLine = iFile.readline()
while sLine:
   try:
      uLine = sLine.decode("gb2312")
   except UnicodeDecodeError:
      sLine = iFile.readline()
      continue
   oFile.write(uLine.encode("utf8"))
   sLine = iFile.readline()
iFile.close()
oFile.close()
