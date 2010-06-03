scripts=~/local/src/cpp1/scripts
candc=~/local/app/candc
corpus=~/local/corpus/ccg

rm $1.ccgbank_deps
echo '#' >>$1.ccgbank_deps
echo '#' >>$1.ccgbank_deps
echo '' >>$1.ccgbank_deps
count=`wc -l $1.pipe|cut -d\  -f1`
for i in `seq 1 $count`;
do
  cat $1.txt | head -$i | tail -1 >$1.txt.part
  if [ `wc -w $1.txt.part|cut -d\  -f1` == 0 ] 
  then 
    echo ''>>$1.ccgbank_deps
  else

    python $scripts/lang/chinese/ctbtools/unbinarize.py -op $1.txt.part >$1.pipe.part
    python $scripts/ccg/pipe.py split $1.pipe.part $1.cat.part $1.pipe.fragmented.part
    $candc/bin/generate -j $candc/src/data/ccg/cats $candc/src/data/ccg/cats/markedup $1.pipe.fragmented.part >$1.ccgbank_deps.part
    python $scripts/ccg/mergefragmenteddeps.py $1.ccgbank_deps.part $1.pipe.fragmented.part >$1.ccgbank_deps.formatted.part
    grep ^[^\#] $1.ccgbank_deps.formatted.part >>$1.ccgbank_deps
    cat $1.cat.part >>$1.ccgbank_deps
    rm $1.cat.part
    rm $1.ccgbank_deps.part
    rm $1.pipe.part
    rm $1.pipe.fragmented.part
    rm $1.ccgbank_deps.formatted.part
  fi
done
$candc/src/scripts/ccg/evaluate $2.stagged $2.ccgbank_deps $1.ccgbank_deps
