#****************************************************************
#
# config.py - configurations
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.03
#
#****************************************************************

class CConfig(object):
   def __init__(self, sFile):
      file = open(sFile)
      line = file.readline()
      while line:
         line = line.strip()
         if not line: 
            line = file.readline()
            continue
         if line[0] == '#':
            line = file.readline()
            continue
         lConfig = line.split('=')
         assert len(lConfig) == 2 # must be written in the format of a = b
         self.__setattr__(lConfig[0].strip(), eval(lConfig[1]))
         line = file.readline()
      file.close()

