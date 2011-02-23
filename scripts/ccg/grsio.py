import sys
import getopt
import re

g_reDep = re.compile('\((\S+) (\S+) (\S+)(?: (\S+))?\)') # (ncmod _ managed_32 But_0)
g_reTok = re.compile('(\S+)\_(\d+)')

def addRel(rel, match, headidx, modidx):
   tok_match = g_reTok.match(match.group(headidx))
   head = tok_match.group(2)
   tok_match = g_reTok.match(match.group(modidx))
   mod = tok_match.group(2)
   return [rel, int(head), int(mod)]

def getRel(match):
   rel = match.group(1)
   if rel == 'conj':
      yield addRel(rel, match, 3, 2)
      return
   elif rel == 'aux':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'det':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'ncmod':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      yield addRel(rel, match, 3, 4)
      return
   elif rel == 'xmod':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      yield addRel(rel, match, 3, 4)
      return
   elif rel == 'cmod':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         yield addRel(rel+'_subtype', match, 3, 2)
      yield addRel(rel, match, 3, 4)
      return
   if rel == 'pmod':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'ncsubj':
      rel_subtype = match.group(4)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'xsubj':
      rel_subtype = match.group(4)
      if rel_subtype == '_':
         pass
      else:
         rel = rel + '_' + rel_subtype
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'csubj':
      rel_subtype = match.group(4)
      assert not rel_subtype == '_'
      rel = rel + '_' + rel_subtype
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'dobj':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'obj2':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'iobj':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'pcomp':
      yield addRel(rel, match, 2, 3)
      return
   elif rel == 'xcomp':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         yield addRel(rel+'_subtype', match, 3, 2)
      yield addRel(rel, match, 3, 4)
      return
   elif rel == 'ccomp':
      rel_subtype = match.group(2)
      if rel_subtype == '_':
         pass
      else:
         yield addRel(rel+'_subtype', match, 3, 2)
      yield addRel(rel, match, 3, 4)
      return
   elif rel == 'ta':
      rel_subtype = match.group(2)
      assert not rel_subtype == '_'
      rel = rel + '_' + rel_subtype
      yield addRel(rel, match, 3, 4)
      return
   else:
      print >>sys.stderr, "type %s unknown" % rel
      raise "exception"

def readSent(path):
   file = open(path)
   bStart = True
   words = []
   links = []
   for line in file:
      # skip the starting comments / empty line
      line = line[:-1]
      if bStart and ( not line or line.startswith('#') ):
         continue
      bStart = False
      # read sentence
      match = g_reDep.match(line)
      if not match:
         if not (not line or line.startswith('<c>')):
            print >>sys.stderr, 'Skipped line:', line
            continue
         assert not line or line.startswith('<c>')
         if not line:
            yield words, links
            words = []
            links = []
         else:
            line = line[4:]
            words = [word.split('|') for word in line.split()]
      else:
         try:
            for link in getRel(match):
               links.append(link)
         except:
            print >>sys.stderr, 'The relation ' + line + ' doesnot conform to grs standard'
   file.close()

if __name__ == '__main__':
   try:
      opts, args = getopt.getopt(sys.argv[1:], '', {})
   except:
      print 'grsio.py input >output'
      sys.exit(1)
   for sent in readSent(args[0]):
      print sent[0]
      for link in sent[1]:
         print link#1]
      print
