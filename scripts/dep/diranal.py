import sys
import depio

def diranal(sent, dHead, dDep, setRoot):
   word_count=0
   for word in sent:
      head = int(word[2])
      pos = _ou(word[1])
      if head == -1:
         setRoot.add(pos)
      else:
         # head
         head_pos = _ou(sent[head][1])
         if not head_pos in dHead:
            dHead[head_pos] = [0,0]
         if head<word_count:
            dHead[head_pos][1] += 1
         else:
            assert head>word_count
            dHead[head_pos][0] += 1
         # dep
         if not pos in dDep:
            dDep[pos] = [0,0]
         if head<word_count:
            dDep[pos][0] += 1
         else:
            assert head>word_count
            dDep[pos][1] += 1
      word_count+=1

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

def _ou(s):
#   return s
   return g_macroNamed[s]

if __name__ == "__main__":
   dHead = {}
   dDep = {}
   setRoot = set()
   for sent in depio.depread(sys.argv[1]):
      diranal(sent, dHead, dDep, setRoot)
   print "head POS with left / right dep"
   for head_pos in dHead:
      print head_pos, dHead[head_pos][0], dHead[head_pos][1]
   print; print "dep POS with left / right head"
   for pos in dDep:
      print pos, dDep[pos][0], dDep[pos][1]
   print; print "the set root"
   print ' '.join(pos for pos in setRoot)
