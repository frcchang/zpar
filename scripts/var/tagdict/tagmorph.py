import sys
import pprint
import gzip
import getopt
import re
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../pos"))
import closed

g_reAlphabet = re.compile('[A-Za-z]{3}[A-Za-z]+') # get four letters

def readlist(path, d):
   for line in path:
      d.append(line.split())

def tagrel(d):
   retval = []
   keys = sorted(d)
   for index, key in enumerate(keys):
      word1 = key[0]
      tag1 = key[1]
      # filter
      if not g_reAlphabet.match(word1):
	 continue
      if tag1 in closed.closedtags:
         continue
      for index2 in range(index+1, len(keys)):
         word2 = keys[index2][0]
         tag2 = keys[index2][1]
         # filter
         if not g_reAlphabet.match(word2):
            continue
         if tag2 in closed.closedtags:
            continue
         if word1[-1] in ['e', 'y']:
            word1 = word1[:-1]
         if word2[-1] in ['e', 'y']:
            word2 = word2[:-1]
         if word1.startswith(word2) or word2.startswith(word1):
            retval.append([word1, tag1, word2, tag2])
   return retval

def printtup(d):
   for key in d.keys():
      print ' '.join(key), d[key]

def printtupsorted(d):
   kys = sorted(d.keys(), key=lambda x:d[x])
   for key in kys:
      print ' '.join(key), d[key]

def printlist(l):
   for item in l:
      print ' '.join(item)

def getgroups(d, cutoff):
   import networkx
   G = networkx.Graph()
   for tg1, tg2 in d:
      G.add_node(tg1)
      G.add_node(tg2)
      G.add_edge(tg1, tg2)
   for tgs in networkx.connected_components(G):
      print ' '.join(tgs)

if __name__ == '__main__':
   command = sys.argv[1]
   if command == 'dic2rel':
      if sys.argv[2].endswith('.gz'):
         file = gzip.open(sys.argv[1])
      else:
         file = open(sys.argv[2])
      d = []
      readlist(file, d)
      r = tagrel(d)
      printlist(r)
   elif command == 'rel2group':
      file = open(sys.argv[2])
      cutoff = 1
      if len(sys.argv) > 3:
         cutoff = int(sys.argv[3])
      d = {}
      readlist(file, d)
      getgroups(d, cutoff)
   else:
      print "Unknown command"
