import depio

def getalllabels(path):
   labels = set()
   for dep in depio.depread(path):
      for word in dep:
         label = word[3]
         labels.add(label)
   return labels
   
if __name__ == '__main__':
   import sys
   if len(sys.argv) < 3:
      print 'getlabels.py dep_path mode'
      print 'mode: all - get all labels; pos  get labels for head pos tag.'
      sys.exit(0)
   path = sys.argv[1]
   mode = sys.argv[2]
   if mode == 'all':
      labels = getalllabels(path)
      for label in labels:
         print label
