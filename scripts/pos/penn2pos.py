import sys
import re
import posio
import os

g_reComment=re.compile(r'=+$|\*x\*')

g_dReplace = {'(' : '-LRB-', ')' : '-RRB-'}
#g_dReplace = {}

def penn2pos(path, sep='/', basenp=False):
   file = open(path)
   sent = []
   nps = [] # nps
   npstart = -1 # last np
   for line in file:
      line = line.strip()
      if not line and sent and sent[-1][1]=='.' or g_reComment.match(line):
         if sent:
            if basenp: yield sent, nps 
            else: yield sent
            sent = []
            nps = []
            assert npstart == -1
         continue
      for word in line.split():
         if word in ['[', ']']:
            if basenp:
               if word == '[':
                  assert npstart == -1
                  npstart = len(sent)
               else:
                  assert npstart != -1
                  nps.append((npstart, len(sent)))
                  npstart = -1
         else:
            word = word.split('/')
            word = ['/'.join(word[:-1]), word[-1]]
            assert len(word) == 2
            word[1] = g_dReplace.get(word[1], word[1])
            if word[1].find('|') != -1:
               word[1] = word[1].split('|')[0]
            sent.append(word)
   file.close()

def batch_penn_to_pos(base, sections):
   sections = sections.split('-')
   assert len(sections) == 1 or len(sections) == 2
   if len(sections) == 1:
      sections = [int(sections[0])]
   else:
      sections = range(int(sections[0]), int(sections[1]))
   for sec in sections:
      path = os.path.join(base, '%02d'%(sec,))
      for file in os.listdir(path):
         assert file.endswith('.pos')
         for sent in penn2pos(os.path.join(path, file)):
            posio.posprint(sent)

def basenp(base, sections):
   sections = sections.split('-')
   assert len(sections) == 1 or len(sections) == 2
   if len(sections) == 1:
      sections = [int(sections[0])]
   else:
      sections = range(int(sections[0]), int(sections[1]))
   for sec in sections:
      path = os.path.join(base, '%02d'%(sec,))
      for file in os.listdir(path):
         assert file.endswith('.pos')
         for sent, nps in penn2pos(os.path.join(path, file), basenp=True):
            posio.posprint(sent)
            print nps

if __name__ == '__main__':
   if len(sys.argv) < 2:
      print 'penn2pos.py command argument'
      print 'commands:'
      print '  penn2pos'
      sys.exit(1)
   command = sys.argv[1]
   if command == 'penn2pos':
      if len(sys.argv) != 4:
         print 'penn2pos.py penn2pos base sections'; sys.exit(0)
      batch_penn_to_pos(sys.argv[2], sys.argv[3])
   if command == 'basenp':
      if len(sys.argv) != 4:
         print 'penn2pos.py basenp base sections'; sys.exit(0)
      basenp(sys.argv[2], sys.argv[3])
