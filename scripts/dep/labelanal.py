import sys
import depio

def labelanal(sent, dLabel, setPOS):
   for word in sent:
      head = int(word[2])
      pos = word[1]
      label = word[3]
      setPOS.add(pos)
      if head == -1:
         #assert label == 'ROOT'
         pass
      else:
         head_pos = sent[head][1]
         setPOS.add(head_pos)
         if not label in dLabel:
            dLabel[label] = {}
         if not head_pos in dLabel[label]:
            dLabel[label][head_pos] = {}
         if not pos in dLabel[label][head_pos]:
            dLabel[label][head_pos][pos] = 0
         dLabel[label][head_pos][pos] += 1


g_macroNamed = {
   "-NONE-" : 'PENN_TAG_NONE', 
   "-BEGIN-" : 'PENN_TAG_BEGIN',
   "-END-":'PENN_TAG_END',
   "$":'PENN_TAG_DOLLAR', "``":'PENN_TAG_L_QUOTE', "''":'PENN_TAG_R_QUOTE', 
   "-LRB-":'PENN_TAG_L_BRACKET', "-RRB-":'PENN_TAG_R_BRACKET', ",":'PENN_TAG_COMMA', 
   ".":'PENN_TAG_PERIOD', ":":'PENN_TAG_COLUM', "#":'PENN_TAG_SHART',
   'CC':'PENN_TAG_CC', 'CD':'PENN_TAG_CD', 'DT':'PENN_TAG_DT', 'EX':'PENN_TAG_EX', 'FW':'PENN_TAG_FW',
   'IN':'PENN_TAG_IN', 'JJ':'PENN_TAG_ADJECTIVE', 'JJR':'PENN_TAG_ADJECTIVE_COMPARATIVE', 'JJS':'PENN_TAG_ADJECTIVE_SUPERLATIVE', 'LS':'PENN_TAG_LS', 
   'MD':'PENN_TAG_MD', 'NN':'PENN_TAG_NOUN', 'NNP':'PENN_TAG_NOUN_PROPER', 'NNPS':'PENN_TAG_NOUN_PROPER_PLURAL', 'NNS':'PENN_TAG_NOUN_PLURAL', 
   'PDT':'PENN_TAG_PDT', 'POS':'PENN_TAG_POS', 'PRP':'PENN_TAG_PRP', 'PRP$':'PENN_TAG_PRP_DOLLAR',
   'RB':'PENN_TAG_ADVERB', 'RBR':'PENN_TAG_ADVERB_COMPARATIVE', 'RBS':'PENN_TAG_ADVERB_SUPERLATIVE', 'RP':'PENN_TAG_RP',
   'SYM':'PENN_TAG_SYM', 'TO':'PENN_TAG_TO', 'UH':'PENN_TAG_UH', 
   'VB':'PENN_TAG_VERB', 'VBD':'PENN_TAG_VERB_PAST', 'VBG':'PENN_TAG_VERB_PROG', 'VBN':'PENN_TAG_VERB_PAST_PARTICIPATE', 'VBP':'PENN_TAG_VERB_PRES', 'VBZ':'PENN_TAG_VERB_THIRD_SINGLE',
   'WDT':'PENN_TAG_WDT', 'WP':'PENN_TAG_WP', 'WP$':'PENN_TAG_WP_DOLLAR', 'WRB':'PENN_TAG_WRB' 
}

def printStats(path):
   def _ou(s):
      return s
   #   return g_macroNamed[s]
   dLabel = {}
   setPOS = set([])
   for sent in depio.depread(path):
      labelanal(sent, dLabel, setPOS)
   print 'Set of labels'
   print ' '.join(dLabel.keys())
   for label in dLabel:
      print label, '==='
      setDeps = set([])
      dPair = dLabel.get(label, {})
      dDepCount={}
      for head in dPair:
         headCount = 0
         dDep = dPair.get(head, {})
         for key in dDep.keys():
            headCount += dDep[key]
            if not key in dDepCount:
               dDepCount[key] = 0
            dDepCount[key]+=dDep[key]
         print head, '(', headCount, ')', ' : ', ' '.join([key+'('+str(dDep[key])+')' for key in dDep.keys()])
         setDeps = setDeps.union(dDep.keys())
      print
      print 'Set of heads: ', ' '.join(map(_ou, dPair.keys()))
      assert set(dDepCount.keys()) == setDeps
      print 'Set of depcounts: ', ' '.join([_ou(key)+'('+str(dDepCount[key])+')' for key in dDepCount.keys()])
      print 'Set of deps: ', ' '.join(map(_ou, setDeps))
      print
      print 'Set of nonheads: ', ' '.join(map(_ou, (setPOS-set(dPair.keys()))))
      print 'Set of nondeps: ', ' '.join(map(_ou, (setPOS-setDeps)))
      print

if __name__ == "__main__":
   import getopt
   optlist, args = getopt.getopt(sys.argv[1:], 'c')
   for opts in optlist:
      if opts[0] == '-c':
         print 'Write code'
         sys.exit(0)
   printStats(args[0])

