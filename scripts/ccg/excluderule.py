import sys

def mapping(a):
   if a == '(S/S)':
      return 'S/S'
   if a == '(S\S)':
      return 'S\S'
   elif a == '(NP/NP)':
      return 'NP/NP'
   elif a == '(NP\NP)':
      return 'NP\NP'
   elif a == '(N/N)':
      return 'N/N'
   elif a == '(N\N)':
      return 'N\N'
   elif a == '(S[pss]\NP)':
      return 'S[pss]\NP'
   elif a == '((S[adj]\NP)\(S[adj]\NP))':
      return '(S[adj]\NP)\(S[adj]\NP)'
   elif a == '(S[adj]\NP)':
      return 'S[adj]\NP'
   elif a == '((S\NP)\(S\NP))':
      return   '(S\NP)\(S\NP)'
   elif a == '((S[pss]\NP)\(S[pss]\NP))':
      return '(S[pss]\NP)\(S[pss]\NP)'
   elif a == '(S[dcl]\S[dcl])':
      return 'S[dcl]\S[dcl]'
   elif a == '((S[dcl]\NP)\(S[dcl]\NP))':
      return '(S[dcl]\NP)\(S[dcl]\NP)'
   elif a == '(((S\NP)\(S\NP))\((S\NP)\(S\NP)))':
      return '((S\NP)\(S\NP))\((S\NP)\(S\NP))'
   elif a == '((S\NP)\((S\NP)/(S[adj]\NP)))':
      return '(S\NP)\((S\NP)/(S[adj]\NP))'
   elif a == '(S[dcl]\NP)':
      return 'S[dcl]\NP'
   elif a == '(S[ng]\NP)':
      return 'S[ng]\NP'
   elif a == '((N/N)\(N/N))':
      return '(N/N)\(N/N)'
   elif a == '(PP\PP)':
      return 'PP\PP'
   elif a == '(S[to]\NP)':
      return 'S[to]\NP'
   elif a == '((NP\NP)\(NP\NP))':
      return '(NP\NP)\(NP\NP)'
   elif a == '((S[pss]\NP)/PP)':
      return '(S[pss]\NP)/PP'
   elif a == '(((S\NP)/(S\NP))\((S\NP)/(S\NP)))':
      return '((S\NP)/(S\NP))\((S\NP)/(S\NP))'
   elif a == '(S[dcl]/NP)':
      return 'S[dcl]/NP'
   elif a == '(S[b]\NP)':
      return 'S[b]\NP'
   elif a == '(S[intj]\S[intj])':
      return 'S[intj]\S[intj]'
   elif a == '((S\NP)/(S\NP))':
      return '(S\NP)/(S\NP)'
   elif a == '(NP[nb]/N)':
      return 'NP[nb]/N'
   elif a == '(NP/(S[dcl]\NP))':
      return 'NP/(S[dcl]\NP)'
   elif a == '((S[ng]\NP)\(S[ng]\NP))':
      return '(S[ng]\NP)\(S[ng]\NP)'
   elif a == '(((S/S)/(S[adj]\NP))\(((S/S)/(S[adj]\NP))/NP))':
      return '((S/S)/(S[adj]\NP))\(((S/S)/(S[adj]\NP))/NP)'
   elif a == '((S[b]\NP)\(S[b]\NP))':
      return '(S[b]\NP)\(S[b]\NP)'
   elif a == '((S[dcl]/NP)/NP)':
      return '(S[dcl]/NP)/NP'
   elif a == '(((S[adj]\NP)/NP)\((S[adj]\NP)/NP))':
      return '((S[adj]\NP)/NP)\((S[adj]\NP)/NP)'
   elif a == '(S\(S/NP))':
      return 'S\(S/NP)'
   elif a == '((S[adj]\NP)\NP)':
      return '(S[adj]\NP)\NP'
   elif a == '((S[adj]\NP)/(S[adj]\NP))':
      return '(S[adj]\NP)/(S[adj]\NP)'
   elif a == '(((S\NP)/(S[adj]\NP))\(((S\NP)/(S[adj]\NP))/NP))':
      return '((S\NP)/(S[adj]\NP))\(((S\NP)/(S[adj]\NP))/NP)'
   elif a == '((S\NP)/(S[adj]\NP))\(((S\NP)/(S[adj]\NP))/NP)':
      return '(S\NP)/(S[adj]\NP))\(((S\NP)/(S[adj]\NP))/NP'
   elif a == '((S\NP)\(((S\NP)/(S[adj]\NP))/NP))':
      return '(S\NP)\(((S\NP)/(S[adj]\NP))/NP)'
   elif a == '((((S\NP)\(S\NP))\((S\NP)\(S\NP)))\(((S\NP)\(S\NP))\((S\NP)\(S\NP))))':
      return '(((S\NP)\(S\NP))\((S\NP)\(S\NP)))\(((S\NP)\(S\NP))\((S\NP)\(S\NP)))'
   elif a == '((S[ng]\NP)/NP)':
      return '(S[ng]\NP)/NP'
   elif a == '((NP\NP)/NP)':
      return '(NP\NP)/NP'
   else:
      return a  

def contains(set1, tuple2):
   def extendlist(x, i):
      if i == len(x):
         return [[]]
      retval = []
      next = extendlist(x, i+1)
      for r in next:
         for element in x[i]:
            retval.append([element]+r)
      return retval
         
   def enumeratetuple(tuple2):
      retval = []
      d = {'NP[conj]' : ['NP\NP'], 'S[pss]\NP[conj]' : ['(S[pss]\NP)\(S[pss]\NP)'], 'S[dcl][conj]' : ['S[dcl]\S[dcl]'], '(S\NP)\(S\NP)[conj]' : ['((S\NP)\(S\NP))\((S\NP)\(S\NP))'], 'S[dcl]\NP[conj]' : ['(S[dcl]\NP)\(S[dcl]\NP)'], '(S\NP)\((S\NP)/(S[adj]\NP))[conj]' : ['(((S\NP)\((S\NP)/(S[adj]\NP)))\((S\NP)\((S\NP)/(S[adj]\NP))))'], 'S[adj]\NP[conj]' : ['(S[adj]\NP)\(S[adj]\NP)'], 'S[dcl]\NP[conj]' : ['(S[dcl]\NP)\(S[dcl]\NP)'], 'S[intj][conj]' : ['S[intj]\S[intj]'], 'S[ng]\NP[conj]' : ['(S[ng]\NP)\(S[ng]\NP)'], 'S[b]\NP[conj]' : ['(S[b]\NP)\(S[b]\NP)'], '(S[adj]\NP)/NP[conj]' : ['((S[adj]\NP)/NP)\((S[adj]\NP)/NP)'], 'PP[conj]' : ['PP\PP']}
#'NP\NP' : ['NP[conj]'], 
#'(S[pss]\NP)\(S[pss]\NP)' : ['S[pss]\NP[conj]'], 
      l=list(tuple2)
      for index in range(len(l)):
         l[index] = [l[index]]
         l[index].extend(d.get(l[index][0], []))
      return [tuple2] + [tuple(e) for e in extendlist(l, 0)]
   
   for tuple1 in enumeratetuple(tuple2):
      if tuple1 in set1:
         return tuple1
   return None

def filterunary(path, exceptpath):
   excepts = set()
   exceptfile = open(exceptpath)
   for exceptline in exceptfile:
      exceptline = exceptline.strip().split()
      assert len(exceptline) == 2
      excepts.add((mapping(exceptline[0]), mapping(exceptline[1])))
   exceptfile.close()

   file = open(path)
   rules = {}
   for line in file:
      line = line.strip().split()
      rules[line[0]] = []
      assert line[1] == ':'
      assert line[2] == '['
      last_index = 2
      for index in range(3, len(line)):
         if line[index] == ']':
            r = contains(excepts, (line[0], line[index-1]))
            if not r:
               rules[line[0]].append(' '.join(line[last_index+1:index]))
            else:
               excepts.remove(r)
            assert index == len(line)-1
         elif line[index] == ',':
            r = contains(excepts, (line[0], line[index-1]))
            if not r:
               rules[line[0]].append(' '.join(line[last_index+1:index]))
            else:
               excepts.remove(r)
            last_index = index
            assert line[index+1] == 'REDUCE'
   file.close()
   if excepts != set():
      for e in excepts:
         print e[0], e[1]
   assert excepts == set()
   for key in rules:
      if rules[key]:
         print '%s\t:\t[ %s ]' % (key, ' , '.join(rules[key]))

def filterbinary(path, exceptpath):
   excepts = set()
   exceptfile = open(exceptpath)
   for exceptline in exceptfile:
      exceptline = exceptline.strip().split(' ')
      assert len(exceptline) == 3
      excepts.add((mapping(exceptline[0]), mapping(exceptline[1]), mapping(exceptline[2])))
   exceptfile.close()

   file = open(path)
   rules = {}
   for line in file:
      line = line.strip().split(' ')
      rules[(line[0],line[2])] = []
      assert line[1] == ','
      assert line[3] == ':'
      assert line[4] == '['
      last_index = 4
      filtered = set()
      for index in range(5, len(line)):
         if line[index] == ']':
            r = contains(excepts, (line[0], line[2], line[index-1]))
            if not r:
               rules[(line[0], line[2])].append(' '.join(line[last_index+1:index]))
            else:
               excepts.remove(r)
            assert index == len(line)-1
#            if line[index-1] in filtered:
#               print >>sys.stderr, line[0], line[2], line[index-1]
            #assert not line[index-1] in filtered
         elif line[index] == ',':
            r = contains(excepts, (line[0], line[2], line[index-1]))
            if not r:
               rules[(line[0], line[2])].append(' '.join(line[last_index+1:index]))
            else:
               
               excepts.remove(r)
            assert not line[index-1] in filtered
            assert line[index+1] == 'REDUCE'
            assert line[index+2] == 'BINARY'
            filtered.add(line[index-1])
            last_index = index
   file.close()
   if excepts != set():
      for key in excepts:
         print key[0], key[1], key[2]
   assert excepts == set()
   for key in rules:
      if rules[key]:
         print '%s , %s\t:\t[ %s ]' % (key[0], key[1], ' , '.join(rules[key]))

if __name__ == '__main__':
   if sys.argv[1] == '-u':
      filterunary(sys.argv[2], sys.argv[3])
   elif sys.argv[1] == '-b':
      filterbinary(sys.argv[2], sys.argv[3])
   else:
      print 'filterrules.py -u/b rule exceptions'
      sys.exit(1)
