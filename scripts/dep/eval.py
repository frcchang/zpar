# -encoding:utf8-
import sys
import depio
import re

g_reP = re.compile(r"^[,?!:;]$|^-LRB-$|^-RRB-$|^[.]+$|^[`]+$|^[']+$|^（$|^）$|^、$|^。$|^！$|^？$|^…$|^，$|^；$|^／$|^：$|^“$|^”$|^「$|^」$|^『$|^』$|^《$|^》$|^一一$")


def eval(output, reference):
   total_uem = 1
   total = 0
   correct_head = 0
   correct_label = 0
   for index, word in enumerate(output):
      ref_word = reference[index]
      assert word[0] == ref_word[0]
      if g_reP.match( word[0] ) :
         continue
      if word[2] == ref_word[2]:
         correct_head += 1
         if word[3] == ref_word[3]:
            correct_label += 1
      else:
         total_uem = 0
      total += 1
   return correct_head, correct_label, total, total_uem

if __name__ == '__main__':
   file_output = depio.depread(sys.argv[1])
   file_ref = depio.depread(sys.argv[2])
   total_sent = 0
   total_uem = 0
   total = 0
   correct_head = 0
   correct_label  =0
   for output in file_output:
      ref = file_ref.next()
      ret = eval(output, ref)
      correct_head += ret[0]
      correct_label += ret[1]
      total += ret[2]
      total_uem += ret[3]
      total_sent += 1
   print float(correct_head)/total, float(correct_label)/total, float(total_uem)/total_sent

