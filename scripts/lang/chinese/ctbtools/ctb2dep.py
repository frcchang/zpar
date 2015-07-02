#****************************************************************
#
# ctb2dep.py - reformat ctb into dependency.
#
# Author: Yue Zhang
#
# Computing lab, University of Oxford. 2008.03
#
#****************************************************************

import fiditer
import fidtree

import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "../../../"))

from tools.encoding.gb2utf import *

class CDependencyNode(object):
   slots = ['id', 'token', 'pos', 'link', 'label']
   def __init__(self, token, pos, id, link, label):
      self.id=id
      self.token=token
      self.pos=pos.split('-')[0]
      self.link=link
      self.label = label

#================================================================

class CHeadRules(object):

   def __init__(self, sFile, sLogs, bLabeled):
      self.m_dRules = {}
      file = open(sFile)
      line = file.readline()
      while line:
         line = line.strip()
         lPos = line.split(":")
         assert len(lPos) == 2
         sHead = lPos[0].strip()
         self.m_dRules[sHead]=[ru.strip().split() for ru in lPos[1].split(";")] # {NP: [[r, NP, VP], [r]]}
         line = file.readline()
      file.close()
      if sLogs != None:
         self.log = open(sLogs, "w")
      else:
         self.log = None
      self.m_bLabeled = bLabeled

   def __del__(self):
      if self.log != None:
         self.log.close()

   def add_link(self, children, head_child):
      """assign link to words after head found"""
      for other_child, tmp in children:
         if other_child != head_child:
            assert other_child.link == -1
            other_child.link = head_child.id

   def add_label(self, constituent, children, head_child):
      """add link and label to words after finding heads"""
      constituent = constituent.split('-')[0]
      for other_child, tmp in children:
         if other_child != head_child:
            assert other_child.link == -1
            other_child.link = head_child.id
            # add label
            sub_cons = tmp.name.split('-')
            if len(sub_cons)>1:
               # subjects and objects
               if 'OBJ' in sub_cons[1:]:
                  other_child.label = 'OBJ'
                  continue
               elif 'SBJ' in sub_cons[1:]:
                  other_child.label = 'SBJ'
                  continue
#               else:
#                  print sub_cons[1], head_child.pos
            # any links from inside prepositional phrase
            if constituent == 'PP':
               if other_child.id > head_child.id:
                  other_child.label = 'POBJ'
               else:
                  other_child.label = 'PMOD'
            elif constituent in ['NP', 'NN']:
               other_child.label = 'NMOD'
            elif constituent == 'VP':
               other_child.label = 'VMOD'
            elif constituent == 'VRD':
               other_child.label = 'VRD'
            elif constituent == 'IP':
               if head_child.pos in ['VV', 'VE', 'VC', 'VA']:
                  other_child.label = 'VMOD'
               else:
                  other_child.label = 'VMOD'
            elif constituent == 'DNP':
               other_child.label = 'DEG'
            elif constituent == 'DVP':
               other_child.label = 'DEV'
            elif constituent == 'LCP':
               other_child.label = 'LC'
            elif constituent in ['QP', 'DP']:
               if head_child.pos == 'CD':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'M':
                  other_child.label = 'M'
               elif head_child.pos == 'OD':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'NN':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'DT':
                  other_child.label = 'AMOD'
            elif constituent == 'PRN':
               other_child.label = 'PRN'
            elif constituent == 'VCP':
               other_child.label = 'VC'
            elif constituent in ['VCD', 'UCP']:
               other_child.label = 'COOR'
            else:
               if head_child.pos in ['VV', 'VC', 'VE', 'VA']:
                  other_child.label = 'VMOD'
               elif head_child.pos in ['NN', 'NR', 'PN', 'NT']:
                  other_child.label = 'NMOD'
               elif head_child.pos == 'CS':
                  other_child.label = 'CS'
               elif head_child.pos == 'AD':
                  other_child.label = 'AMOD'
               elif head_child.pos == 'JJ':
                  other_child.label = 'AMOD'
               elif head_child.pos == 'DEC':
                  other_child.label = 'DEC'
               elif head_child.pos == 'M':
                  other_child.label = 'M'
               elif head_child.pos == 'CD':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'OD':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'DT':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'PU':
                  other_child.label = 'NMOD'
               elif head_child.pos == 'P':
                  other_child.label = 'PMOD'
               elif head_child.pos == 'IJ':
                  other_child.label = 'PMOD'
               elif head_child.pos == 'LC':
                  other_child.label = 'LC'
            if other_child.label == '':
               other_child.label = '???'

   def add_label_simple(self, constituent, children, head_child):
      """add link and label to words after finding heads"""
      constituent = constituent.split('-')[0]
      for other_child, tmp in children:
         if other_child != head_child:
            assert other_child.link == -1
            other_child.link = head_child.id
            # add label
            sub_cons = tmp.name.split('-')
            if len(sub_cons)>1:
               # subjects and objects
               if 'OBJ' in sub_cons[1:]:
                  other_child.label = 'OBJ'
               elif 'SBJ' in sub_cons[1:]:
                  other_child.label = 'SBJ'
               else:
                  other_child.label = 'DEP'
#                  print sub_cons[1], head_child.pos
            else:
               # any links from inside prepositional phrase
               other_child.label = 'DEP'

   def find_head(self, node, lItems):
      """find head"""
      if node.name == '-NONE-':
         return None
      if node.type == "token":
         if node.name == 'NP':
            node.name = 'NN'
         if node.name == 'VP':
            node.name = 'VV'
         elif node.name == 'X':
            node.name = 'NR'
         node.token = '-'.join(node.token.split(' '))
         lItems.append(CDependencyNode(node.token, node.name, len(lItems), -1, ''))
         return lItems[-1]
      else:
         assert node.type == "constituent"
         if node.name == 'NN':
            node.name = 'NP'
         lChildren = [self.find_head(child_node, lItems) for child_node in node.children]
         lZipped = zip(lChildren, node.children)
         lZipped = filter(lambda x: x[0] != None, lZipped)
         if lZipped == []:
            return None
         sLabel = node.name.split("-")[0] # NP-PN
         sLabel = sLabel.split("=")[0]
         for lRuleSet in self.m_dRules.get(sLabel, []):
            assert lRuleSet[0] in ("l", "r", "le", "re")
            lTemp = lZipped[:] # (dep node, constituent)
            if lRuleSet[0] in ("r", "re"):
               lTemp.reverse()
            bFound = False
            if len(lRuleSet) == 1:
               head_child = lTemp[0][0]
               bFound = True
            else:
               if lRuleSet[0] in ("le", "re"):
                  for child, child_node in lTemp:
                     if not child_node.name.split("-")[0] in lRuleSet[1:]:
                        head_child = child
                        bFound = True
                        break
               else:
                  for sHead in lRuleSet[1:]:
                     for child, child_node in lTemp:
                        if child_node.name.split("-")[0]==sHead:
                           head_child = child
                           bFound = True
                           break
                     if bFound:
                        break
            if bFound:
               break
         else:
            if self.log != None:
               print >> self.log, "can't find a rule for " + sLabel + " with " + ", ".join([child_node.name for child_node in node.children])
            head_child = lZipped[-1][0]
         if self.m_bLabeled:
            self.add_label(node.name, lZipped, head_child)
         else:
            self.add_link(lZipped, head_child)
         return head_child

   def process(self, sSentence):
      # don't process empty sentences
      if sSentence.strip() == "":
         return
      # find the cfg node
      head = fidtree.parse_object(sSentence)
      if type(head) == list:
         lHead = head
      else:
         lHead = [head]
      # output the dep node
      for head in lHead:
         lTokens = []
         head_child = self.find_head(head, lTokens)
         if self.m_bLabeled:
            head_child.label = 'ROOT'
         for tokn in lTokens:
            if self.m_bLabeled:
               print "\t".join([gb2utf(tokn.token), tokn.pos, str(tokn.link), tokn.label])
            else:
               print "\t".join([gb2utf(tokn.token), tokn.pos, str(tokn.link)])
         print # empty line

#================================================================

if __name__ == '__main__':
   from tools import config
   if len(sys.argv) < 3:
      print "\nUsage: ctb2dep.py rule_file config_file [log_file] > output\n"
      sys.exit(1)
   sLogs = None
   if len(sys.argv) == 4:
      sLogs = sys.argv[3]
   cf = config.CConfig(sys.argv[2])
   rule = CHeadRules(sys.argv[1], sLogs, cf.labeled)
   fiditer.sentence_iterator(rule.process, cf.directory, cf.range)
