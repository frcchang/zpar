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

def gb2utf(s):
   try:
      return s.decode("gb2312").encode("utf8")
   except Exception, err:
      return "????????"

#
# Main entry
#
if __name__ == '__main__':
   iFile = open(sys.argv[1])
   oFile = open(sys.argv[2], "w")
   sLine = iFile.readline()
   while sLine:
      try:
         uLine = sLine.decode("gb2312")
      except : 
         sLine = iFile.readline()
         continue
      oFile.write(uLine.encode("utf8"))
      sLine = iFile.readline()
   iFile.close()
   oFile.close()
