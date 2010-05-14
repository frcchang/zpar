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
      self.pos=pos
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
               print sub_cons[1]
               if sub_cons[1] == 'OBJ':
                  other_child.label = 'OBJ'
               elif sub_cons[1] == 'SBJ':
                  other_child.label = 'SBJ'
            else:
               if sub_cons[0] == 'ADJP':
                  other_child.label = 'ADJ'
               elif sub_cons[0] in ['NP', 'NN', 'NR']:
                  if constituent == 'PP':
                     if other_child.id > head_child.id:
                        other_child.label = 'POBJ'
                     else:
                        other_child.label = '???'
                  elif constituent == 'LCP':
                     other_child.label = 'LC'
                  elif constituent in ['NP', 'NN']:
                     other_child.label = 'NMOD'
                  else:
                     other_child.label = '??'

   def find_head(self, node, lItems):
      """find head"""
      if node.name == '-NONE-':
         return None
      if node.type == "token":
         lItems.append(CDependencyNode(node.token, node.name, len(lItems), -1, ''))
         return lItems[-1]
      else:
         assert node.type == "constituent"
         lChildren = [self.find_head(child_node, lItems) for child_node in node.children]
         lZipped = zip(lChildren, node.children)
         lZipped = filter(lambda x: x[0] != None, lZipped)
         if lZipped == []:
            return None
         sLabel = node.name.split("-")[0] # NP-PN
         sLabel = sLabel.split("=")[0]
         for lRuleSet in self.m_dRules.get(sLabel, []):
            assert lRuleSet[0] in ("l", "r")
            lTemp = lZipped[:] # (dep node, constituent)
            if lRuleSet[0] == "r":
               lTemp.reverse()
            bFound = False
            if len(lRuleSet) == 1:
               head_child = lTemp[0][0]
               bFound = True
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
         self.find_head(head, lTokens)
         for tokn in lTokens:
            if self.m_bLabeled:
               print gb2utf("\t".join([tokn.token, tokn.pos, str(tokn.link), tokn.label]))
            else:
               print gb2utf("\t".join([tokn.token, tokn.pos, str(tokn.link)]))
         print # empty line

#================================================================

if __name__ == '__main__':
   import config
   if len(sys.argv) < 3:
      print "\nUsage: ctb2dep.py rule_file config_file [log_file] > output\n"
      sys.exit(1)
   sLogs = None
   if len(sys.argv) == 4:
      sLogs = sys.argv[3]
   cf = config.CConfig(sys.argv[2])
   rule = CHeadRules(sys.argv[1], sLogs, cf.labeled)
   fiditer.sentence_iterator(rule.process, cf.directory, cf.range)
