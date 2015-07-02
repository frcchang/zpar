# 
# utf2gb - switch encoding between text
#
# Yue Zhang 2006
#
g_sWelcome = """
gb2utf - switch encoding between text

Usage:  utf2gb input output

Author: Yue Zhang, 2006
"""
import sys

#
# Main entry
#
iFile = open(sys.argv[1])
oFile = open(sys.argv[2], "w")
uLine = iFile.readline().decode("utf-8")
while uLine:
   try:
      sLine = uLine.encode("gb2312")
   except UnicodeEncodeError:
      uLine = iFile.readline()
      continue
   oFile.write(sLine)
   uLine = iFile.readline().decode("utf-8")
iFile.close()
oFile.close()
