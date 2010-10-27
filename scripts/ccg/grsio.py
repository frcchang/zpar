import sys
import getopt
import re

g_reDep = re.compile('\((\S+) (\S+) (\S+)(?: (\S+))?\)') # (ncmod _ managed_32 But_0)
g_reTok = re.compile('(\S+)\_(\d+)')

def getRel(match):
   rel = match.group(1)
   if rel == 'conj':
      tok_match = g_reTok.match(match.group(3))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(2))
      mod = tok_match.group(2)
   elif rel == 'aux':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'det':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'ncmod':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(3))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(4))
      mod = tok_match.group(2)
   elif rel == 'xmod':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(4))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'cmod':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(3))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(4))
      mod = tok_match.group(2)
   if rel == 'pmod':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'ncsubj':
      rel_subtype = match.group(4)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'xsubj':
      rel_subtype = match.group(4)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'csubj':
      rel_subtype = match.group(4)
      assert not rel_subtype == '_'
      rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'dobj':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'obj2':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'iobj':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'pcomp':
      tok_match = g_reTok.match(match.group(2))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(3))
      mod = tok_match.group(2)
   elif rel == 'xcomp':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(3))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(4))
      mod = tok_match.group(2)
   elif rel == 'ccomp':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(3))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(4))
      mod = tok_match.group(2)
   elif rel == 'ta':
      rel_subtype = match.group(2)
      assert not rel_subtype == '_'
      rel = rel + '_' + rel_subtype
      tok_match = g_reTok.match(match.group(3))
      head = tok_match.group(2)
      tok_match = g_reTok.match(match.group(4))
      mod = tok_match.group(2)
   else:
      mod = '0'
      head = '0'
   return [rel, int(mod), int(head)]

def readSent(path):
   file = open(path)
   bStart = True
   words = []
   links = []
   for line in file:
      # skip the starting comments / empty line
      line = line[:-1]
      print line
      if bStart and ( not line or line.startswith('#') ):
         continue
      bStart = False
      # read sentence
      match = g_reDep.match(line)
      if not match:
         print line
         assert not line or line.startswith('<c>')
         if not line:
            yield words, links
            words = []
            links = []
         else:
            line = line[4:]
            words = [word.split('|') for word in line.split()]
      else:
         links.append(getRel(match))
   file.clse()

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], '', {})
   except:
      print 'gs2conll.py input >output'
      sys.exit(1)
   for sent in readSent(args[0]):
      print sent[0]
      for link in sent[1]:
         print link#1]
      print
