import msuper
import parsebin
import sys

def patchpos(parse, super):
   S = msuper.superread(super)
   for p in parsebin.fromFile(parse):
      s = S.next()
      t = [w[1] for w in s]
      p.pos = t
      print p.root

if __name__ == '__main__':
   patchpos(sys.argv[1], sys.argv[2])
