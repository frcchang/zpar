scripts=~/local/src/cpp1/scripts
candc=~/local/app/candc

#python $scripts/lang/chinese/ctbtools/unbinarize.py -op $1 >$1.pipe
#rm $i.ccgbank_deps
#count = `wc -l $1.pipe`
#for i in `seq 1 $count`
#do
#  cat $1.pipe | head -$i | tail -1 >$1.pipe.part
  python $scripts/ccg/pipe.py split $1.pipe.part $1.cat.part $1.pipe.fragmented.part
  $candc/bin/generate -e $candc/src/data/ccg/cats $candc/src/data/ccg/cats/markedup $1.pipe.fragmented.part >$1.ccgbank_deps.part
  grep ^[^\#] $1.ccgbank_deps.part >>$1.ccgbank_deps
  cat $1.cat.part >>$1.ccgbank_deps
#  rm $1.cat.part
#  rm $1.ccgbank_deps.part
#  rm $1.pipe.part
#done
