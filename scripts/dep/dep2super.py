import sys
import depio
import dep
import getopt

def toSuperTagForNode(node):
   retval = []
   for left_child in node.left_children:
      retval.extend(toSuperTagForNode(left_child))
   word = []
   word.append(node.token); word.append(node.pos)
   if node.parent == None:
      word.append('0')
      word.append('0')
   elif node.parent.word_index > node.word_index:
      word.append('0')
      word.append('1')
   else:
      assert node.parent.word_index < node.word_index
      word.append('1')
      word.append('0')
   if len(node.left_children) == 0:
      word.append('0')
   else:
      word.append('1')
   if len(node.right_children) == 0:
      word.append('0')
   else:
      word.append('1')
   if node.parent == None:
      word.append('1')
   else:
      word.append('0')
   retval.append(word)
   for right_child in node.right_children:
      retval.extend(toSuperTagForNode(right_child))
   return retval

def toSuperTag(tree):
   return toSuperTagForNode(tree.root)

def encode(hl, hr, dl, dr, rt):
   return str( (int(hl)<<0) | (int(hr)<<1) | (int(dl)<<2) | (int(dr)<<3) | (int(rt)<<4) )

if __name__== '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "o:")
   if len(args) < 1:
      print 'reorder [-o hl|hr|ml|mr|h|m|hm] input'
      sys.exit(0)
   sOutput = 'hm'
   for opt, val in opts:
      if opt == '-o':
         sOutput = val
   for sent in depio.depread(args[0]):
      dept = dep.CDep(sent)
      supertags = toSuperTag(dept)
      if sOutput == 'hm':
         print ' '.join(['|'.join([word[0], word[1], encode(word[2], word[3], word[4], word[5], word[6])]) for word in supertags])
      elif sOutput == 'h':
         print ' '.join(['|'.join([word[0], word[1], encode(word[2], word[3], 0, 0, 0)]) for word in supertags])
      elif sOutput == 'm':
         print ' '.join(['|'.join([word[0], word[1], encode(0, 0, word[4], word[5], 0)]) for word in supertags])
      elif sOutput == 'hl':
         print ' '.join(['|'.join([word[0], word[1], word[2]]) for word in supertags])
      elif sOutput == 'hr':
         print ' '.join(['|'.join([word[0], word[1], word[3]]) for word in supertags])
      elif sOutput == 'ml':
         print ' '.join(['|'.join([word[0], word[1], word[4]]) for word in supertags])
      elif sOutput == 'mr':
         print ' '.join(['|'.join([word[0], word[1], word[5]]) for word in supertags])
