#encoding=utf8

import sys

def mapping(word):
   d = {'(' : '（', ')' : '）', ':' : '：', ';' : '；', '"' : '“', ',' : '，', '.' : '。'}
   return d.get(word, word)

def translate(sent):
   sent = sent.split()
   r = []
   for word in sent:
      r.append(mapping(word))
   return ' '.join(r)

if __name__ == '__main__':
   path = sys.argv[1]
   file = open(path)
   for line in file:
      print translate(line)
   file.close()
