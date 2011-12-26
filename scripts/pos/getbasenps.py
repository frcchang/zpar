import sys
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'tools'))
import posio
import reader

def getbasenps(pospath, pennpath):
   symbols = reader.readsymbols(pennpath)
   n = 9
   for possent in posio.posread(pospath):
      base_nps = []
      for index in range(len(possent)):
#         possymbol = '/'.join(possent[index])
         symbol = symbols.next().replace('(', '-LRB-').replace(')', '-RRB-').replace('{', '-LCB-').replace('}', '-RCB-')
         lastn = 0
         while symbol[:symbol.rfind('/')] != possent[index][0]:
            if lastn != n:
               posio.posprint(possent)
               print n
               lastn = n
            print symbol
            if symbol == '[':
               assert not base_nps or base_nps[-1][1] != None
               base_nps.append([index, None])
            elif symbol == ']':
               if base_nps:
                  assert base_nps and base_nps[-1][1] == None
                  base_nps[-1][1] = index
	    symbol = symbols.next().replace('(', '-LRB-').replace(')', '-RRB-').replace('{', '-LCB-').replace('}', '-RCB-')
      if base_nps and base_nps[-1][1] == None:
#	 symbol = symbols.next().replace('(', '-LRB-').replace(')', '-RRB-').replace('{', '-LCB-').replace('}', '-RCB-')
#         assert symbol == ']'
	 base_nps[-1][1] = index
      assert not base_nps or base_nps[-1][1]!=None
      print base_nps
      n += 1

if __name__ == '__main__':
   if len(sys.argv) != 3:
      print 'getbasenps.py pos_file penn_file >output_file'
      sys.exit(0)
   getbasenps(sys.argv[1], sys.argv[2])
