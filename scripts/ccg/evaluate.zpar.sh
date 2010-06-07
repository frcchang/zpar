scripts=~/local/src/cpp1/scripts
candc=~/local/app/candc
corpus=~/local/corpus/ccg
scratch=./scratch

if [ -e $scratch ]
then
  echo $scratch dir already exists, exiting now
  exit
fi

mkdir $scratch

rm $1.ccgbank_deps
echo '#' >>$1.ccgbank_deps
echo '#' >>$1.ccgbank_deps
echo '' >>$1.ccgbank_deps
count=`wc -l $1.txt|cut -d\  -f1`
for i in `seq 1 $count`;
do
  echo sentence $i
  cat $1.txt | head -$i | tail -1 >$scratch/$1.txt.part.$i
  if [ `wc -w $scratch/$1.txt.part.$i|cut -d\  -f1` == 0 ] 
  then 
    echo parser failed.
    echo ''>>$1.ccgbank_deps
  else
    
    python $scripts/lang/chinese/ctbtools/unbinarize.py -op $scratch/$1.txt.part.$i >$scratch/$1.pipe.part.$i
    python $scripts/ccg/pipe.py split $scratch/$1.pipe.part.$i $scratch/$1.cat.part.$i $scratch/$1.pipe.fragmented.part.$i
    $candc/bin/generate -j $candc/src/data/ccg/cats $candc/src/data/ccg/cats/markedup $scratch/$1.pipe.fragmented.part.$i >$scratch/$1.ccgbank_deps.part.$i
    python $scripts/ccg/mergefragmenteddeps.py $scratch/$1.ccgbank_deps.part.$i $scratch/$1.pipe.fragmented.part.$i $scratch/$1.cat.part.$i >$scratch/$1.ccgbank_deps.formatted.part.$i
    if [ `$candc/bin/generate -T $candc/src/data/ccg/cats/ $candc/src/data/ccg/cats/markedup $scratch/$1.pipe.fragmented.part.$i | grep '__PARSE_FAILED__' | wc -l | cut -d\  -f1` == 0 ]
#    if [ `wc -w $scratch/$1.ccgbank_deps.formatted.part.$i|cut -d\  -f1` == 0 ] 
    then
      grep ^[^\#] $scratch/$1.ccgbank_deps.formatted.part.$i >>$1.ccgbank_deps
      cat $scratch/$1.cat.part.$i >>$1.ccgbank_deps
    else 
      echo bin/generator failed
      echo '' >>$1.ccgbank_deps
    fi
#    rm $1.cat.part
#    rm $1.ccgbank_deps.part
#    rm $1.pipe.part
#    rm $1.pipe.fragmented.part
#    rm $1.ccgbank_deps.formatted.part
  fi
done
$candc/src/scripts/ccg/evaluate $2.stagged $2.ccgbank_deps $1.ccgbank_deps
