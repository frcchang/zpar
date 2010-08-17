import sys
import prolog
import ccgbank_format
import pipe
import getopt

format = ccgbank_format

FORM=2
POS=4
SUPER=5
HEAD=6

def iter(node, retval):
   if node.type == 'token':
      assert node.start_index == node.end_index
      retval[node.start_index][5] = node.supercategory
      return node.start_index
   assert node.left 
   left_index = iter(node.left, retval)
   if not node.right:
      return left_index
   right_index = iter(node.right, retval)
   # append
   if node.head_left:
      if node.supercategory != '-NONE-': retval[right_index][HEAD] = left_index
      return left_index
   else:
      if node.supercategory != '-NONE-': retval[left_index][HEAD] = right_index
      return right_index

def conll_print(tree):
   retval = [list(item) for item in  tree.tokens]
   for word in retval:
      word.append('')
      word.append(-1)
   iter(tree.root, retval)
   id = 0
   for word in retval:
      print '\t'.join([str(id), word[FORM], word[POS], '_', word[SUPER], str(word[HEAD]), '_', '_', '_'])
      id += 1
   print

if __name__ == "__main__":
   hint = "usage: parsetree2BEtag.py [--input prolog/pipe/ccgbank] file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:", ["input="])
   format = pipe
   for opt, val in optlist:
      if opt == "--input" or opt == '-i':
         if val == "prolog":
            format = prolog
         elif val == 'ccgbank':
            format = ccgbank_format
         else:
            format = pipe
   file = open(args[0])
   tree = format.LoadTree(file)
   while tree:
      conll_print(tree)
      tree = format.LoadTree(file)
   file.close()
