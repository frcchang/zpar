import sys
import pprint
import gzip

def readdicttup(path, d):
   for line in path:
      o1, o2, count = line.split()
      d[o1, o2] = int(count)

def tagrel(d, detail=False):
   retval = {}
   tags = {}
   for word, tag in d:
      if not tag in tags:
         tags[tag] = set()
      tags[tag].add(word)
   for tag1 in tags:
      for tag2 in tags:
         if tag1 == tag2:
            continue
         elif tag1 < tag2:
            tag_pair = tag1, tag2
         else:
            tag_pair = tag2, tag1
         if detail:
            dtl = ' '.join(tags[tag1].intersection(tags[tag2]))
            if dtl:
               retval[tag_pair] = dtl
         else:
            num = len(tags[tag1].intersection(tags[tag2]))
            if num:
               retval[tag_pair] = num
   return retval

def printtup(d):
   for key in d.keys():
      print ' '.join(key), d[key]

def printtupsorted(d):
   kys = sorted(d.keys(), key=lambda x:d[x])
   for key in kys:
      print ' '.join(key), d[key]

def getgroups(d, cutoff):
   import networkx
   G = networkx.Graph()
   for tg1, tg2 in d:
      if d[tg1, tg2] >= cutoff:
         G.add_node(tg1)
         G.add_node(tg2)
         G.add_edge(tg1, tg2)
   for node in G.nodes():
      print node, G.neighbors(node)
#   for tgs in networkx.connected_components(G):
#      print ' '.join(tgs)

if __name__ == '__main__':
   command = sys.argv[1]
   if command == 'dic2rel':
      if sys.argv[2].endswith('.gz'):
         file = gzip.open(sys.argv[1])
      else:
         file = open(sys.argv[2])
      d = {}
      readdicttup(file, d)
      r = tagrel(d)
      printtupsorted(r)
   elif command == 'rel2group':
      file = open(sys.argv[2])
      cutoff = 1
      if len(sys.argv) > 3:
         cutoff = int(sys.argv[3])
      d = {}
      readdicttup(file, d)
      getgroups(d, cutoff)
   elif command == 'dic2relex': # lexicalized
      if sys.argv[2].endswith('.gz'):
         file = gzip.open(sys.argv[1])
      else:
         file = open(sys.argv[2])
      d = {}
      readdicttup(file, d)
      r = tagrel(d, True)
      printtup(r)
   else:
      print "Unknown command"
