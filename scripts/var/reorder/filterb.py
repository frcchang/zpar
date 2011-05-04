import sys
import getopt
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../dep"))
import depio
import dep
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../ml"))
import brute

#========================================

def filter(dep, align):
   l = []
   for j in brute.listOrder(dep):
      a = align.get(j, [])
      if a:
         l.append(a[0])
      else:
         l.append(-1)

   total = 0
   count = 0
   for i in range(len(l)):
      if l[i] != -1:
         for j in range(i+1., len(l)):
            if l[j] != -1:
               total += 1
               if l[i] > l[j]:
                  count += 1

   if count > float(total) * 0.1:
      print 0, dep.toRaw()
   else:
      print dep.toRaw()

#========================================


#========================================

if __name__ == '__main__':
   opts, args = getopt.getopt(sys.argv[1:], "")
   if len(args) < 1:
      print 'adapt input align'
      sys.exit(0)

   # get parameter
   if len(args) != 2:
      print "The alignment file must be provided"
      sys.exit(0)
   sInput = args[0]
   sAlign = args[1]

   # input
   alignFile = brute.readAlign(sAlign) 

   for tree in depio.depread(sInput):
      align = alignFile.next()
      dept = dep.CDep(tree)
      brute.reorder(dept, align, None, False)
      filter(dept, align)
