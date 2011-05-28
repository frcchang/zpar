import sys
import getopt
import os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../dep"))
import depio
import dep
import brute

#========================================

def filter(tree, align):
   dept = dep.CDep(tree)
   brute.reorder(dept, align, None, False)
   l = []
   for j in brute.listOrder(dept):
      a = align.get(j, [])
      if a:
         l.append(a[0])
      else:
         l.append(-1)

   total = 0 # the total number of links
   count = 0 # the number of crossing links
   empty = 0 # the number of no align word
   for i in range(len(l)):
      if l[i] != -1:
         for j in range(i+1, len(l)):
            if l[j] != -1:
               total += 1
               if l[i] > l[j]:
                  count += 1
      else:
         empty += 1

   dept = dep.CDep(tree) # comment this if reordered out desired
   if empty > float(len(l)) * 0.3:
      print 0, 0, dept.toRaw()
   elif count > float(total) * 0.3:
      print 0, dept.toRaw()
   else:
      print dept.toRaw()

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
      filter(tree, align)
