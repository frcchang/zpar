import sys
import prolog
import pipe
import getopt
import random

format = pipe
token_index = 2
pos_index = 4

def Iter(node, cons):
   assert node
   if node.end_index - node.start_index > 1:
      cons.append([str(node.start_index), str(node.end_index), node.supercategory])
   if node.left:
      Iter(node.left, cons)
   if node.right: 
      Iter(node.right, cons)

def ToCon(tree, positive, negative):
   cons = []
   Iter(tree.root, cons)
   size = len(tree.tokens)
   if negative:
      negatives = [1]*(size*size)
   count = 0
   for item in cons:
      if positive == None or item[2] == positive:
         yield item
         if negative:
            negatives[int(item[0])*size+int(item[1])]=0
         count += 1
   if negative == 'random':
      while count:
         i = random.randint(0, size-1)
         j = random.randint(0, size-1)
         if negatives[i*size+j]:
            yield [str(i), str(j), '-NONE-']
            count -= 1
            negatives[i*size+j]=0

if __name__ == "__main__":
   hint = "usage: parsetree2constitu.py [-i prolog/pipe] [-o index/tokens] [-n random] [-p tag] file"
   optlist, args = getopt.getopt(sys.argv[1:], "i:o:n:p:", ["input="])
   format = pipe
   negative = None
   positive = None
   output = 'index'
   for opt, val in optlist:
      if opt == "-i":
         if val == "prolog":
            format = prolog
            token_index = 2
            pos_index = 3
         else:
            format = pipe
            token_index = 2
            pos_index = 4
      if opt == '-o':
         output = val
      if opt == '-n':
         if val == 'random':
            negative = val
      if opt == '-p':
         positive = val
   file = open(args[0])
   lastid = 0
   tree = format.LoadTree(file)
   while tree:
      for y in range(tree.id-lastid-1):
         print
      lastid = tree.id
      words = ['|'.join([token[2],token[4]]) for token in tree.tokens]
      print ' '.join(words)
      for constituent in ToCon(tree, positive, negative):
         if output == 'index':
            print ' '.join(constituent)
         elif output == 'tokens':
            print constituent[2] + ': ' + ' '.join([token[2] for token in tree.tokens][int(constituent[0]):int(constituent[1])])
      print
      tree = format.LoadTree(file)
   file.close()
