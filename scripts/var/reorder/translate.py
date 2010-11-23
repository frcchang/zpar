#encoding=utf8

import sys

d = {'(' : '（', ')' : '）', ':' : '：', ';' : '；', '"' : '“', ',' : '，', '.' : '。'}

def mapping(word, d):
   return d.get(word, word)

def unmapping(d):
   u = {}
   for key in d:
      u[d[key]] = key
   return u

def translate(sent):
   sent = sent.split()
   r = []
   for word in sent:
      r.append(mapping(word, d))
   return ' '.join(r)

def detranslate(sent):
   sent = sent.split()
   r = []
   for word in sent:
      r.append(mapping(word, unmapping(d)))
   return ' '.join(r)

if __name__ == '__main__':
   if len(sys.argv) != 3:
      print 'translate.py d|u input'
      sys.exit(1)
   path = sys.argv[2]
   func = sys.argv[1]
   if func == 'd': 
      f = translate
   elif func == 'u':
      f = detranslate
   else:
      print 'translate.py d|u input'
      sys.exit(1)
   file = open(path)
   for line in file:
      print f(line)
   file.close()
