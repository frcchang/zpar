import sys
import depio

def simplelabelanal(sent, dLabel, setPOS):
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

#===================================================================

def printStats(path):
   def _ou(s):
      return s
   #   return g_macroNamed[s]
   dLabel = {}
   setPOS = set([])
   for sent in depio.depread(path):
      simplelabelanal(sent, dLabel, setPOS)
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

#===================================================================

g_freqCutoff = 0.01

def labelanal(sent, dLabel, setPOS):
   for word in sent:
      head = int(word[2])
      pos = word[1]
      label = word[3]
      setPOS.add(pos)
      if head == -1:
         pass
      else:
         head_pos = sent[head][1]
         setPOS.add(head_pos)
         if not label in dLabel:
            dLabel[label] = {}
         if not (head_pos, pos) in dLabel:
            dLabel[label][(head_pos, pos)] = 0
         dLabel[label][(head_pos, pos)] += 1

def writeCppCode(path):
   def _label(s):
      return 'PENN_DEP_'+s.upper()
   def _pos(s):
      return g_macroNamed[s]
   dLabel = {}
   setPOS = set([])
   for sent in depio.depread(path):
      labelanal(sent, dLabel, setPOS)
   # write header
   print '#include "tags.h"'
   print '#ifdef LABELED'
   print '#include "dependency/label/penn.h"'
   print '#endif'
   print
   print 'namespace english {'
   print '#ifdef LABELED'
   print 'inline bool canAssignLabel(const vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&label) {'
   print '   assert(head==DEPENDENCY_LINK_NO_HEAD||head>=0); // correct head'
   print '   assert(dep>=0);'
   print '   // if the head word is none, only ROOT'
   print '   if (head==DEPENDENCY_LINK_NO_HEAD) {'
   print '      if (label.code()==PENN_DEP_ROOT) '
   print '         return true;'
   print '      return false;'
   print '   }'
   print '      // for each case'
   print '   const unsigned &head_pos = sent[head].tag.code();'
   print '   const unsigned &dep_pos = sent[dep].tag.code();'
   print '   assert(head!=DEPENDENCY_LINK_NO_HEAD);'
   print '   if (label == PENN_DEP_ROOT) // now head is not DEPENDENCY_LINK_NO_HEAD'
   print '      return false;'
   # for each label
   nTotalRules=0
   nLabel=0
   for label in dLabel:
      # print condition
      if nLabel == 0:
         print "   if (label==%s) {" % _label(label)
      else:
         print "   else if (label==%s) {" % _label(label)
      nLabel +=1
      # collect statistics
      dHeadCount={} # head : count
      dDepCount={} # dep : count
      nTotalCount=0 # arc
      dEntry = dLabel.get(label, {}) # head, dep : count
      for key in dEntry:
         head = key[0]
         dep = key[1]
         if not head in dHeadCount:
            dHeadCount[head] = 0
         dHeadCount[head] += dEntry[key]
         if not dep in dDepCount:
            dDepCount[dep] = 0
         dDepCount[dep] += dEntry[key]
         nTotalCount += dEntry[key]
      # write head condition
      threshold = 1 # g_freqCutoff * nTotalCount
      nCount=0
      for pos in setPOS:
         if dHeadCount.get(pos, 0) < threshold:
            if nCount == 0:
               print "      if ( head_pos==%s" % _pos(pos)
            else:
               print "           || head_pos==%s" % _pos(pos)
            nCount += 1
      for pos in setPOS:
         if dDepCount.get(pos, 0) < threshold:
            if nCount == 0:
               print "      if ( dep_pos==%s" % _pos(pos)
            else:
               print "           || dep_pos==%s" % _pos(pos)
            nCount += 1
      if nCount>0:
         print '         ) return false;'
      nTotalRules+=nCount
      # finish condition
      print "   }"
   # write footer
   print "   // total number of rules are %d." % nTotalRules
   print "   return true;"
   print '}'
   print '#endif'
   print
   print 'inline const bool hasLeftHead(const unsigned &tag) {'
   print '   return true;'
   print '}'
   print
   print 'inline const bool hasRightHead(const unsigned &tag) {'
   print '   return true;'
   print '}'
   print 'inline const bool canBeRoot(const unsigned &tag) {'
   print '   return true;'
   print '}'
   print '}'

if __name__ == "__main__":
   import getopt
   optlist, args = getopt.getopt(sys.argv[1:], 'c')
   for opts in optlist:
      if opts[0] == '-c':
         writeCppCode(args[0])
         sys.exit(0)
   printStats(args[0])

