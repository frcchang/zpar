training_file=train.input

rm test_db

zpar=../zpar
candc=../app/candc

for i in `seq 1 10`; do
  $zpar/dist/generic.conparser/train train.txt  test_db 1 -c$training_file -brules.binary -urules.unary
  $zpar/dist/generic.conparser/conparser input.txt output.txt test_db -ic -b
  # evalb
  python $zpar/scripts/lang/chinese/ctbtools/unbinarize.py -ddict.txt output.txt >output.escape
  python skipemptylines.py reference.escape output.escape reference.tmp output.tmp
  EVALB/evalb reference.tmp output.tmp
  # gr
  python skipemptylines.py reference.txt output.txt reference.skipped output.skipped >>log.$i
  python $zpar/scripts/lang/chinese/ctbtools/unbinarize.py -op reference.skipped >reference.pipe.skipped
  python $zpar/scripts/lang/chinese/ctbtools/unbinarize.py -op output.skipped >output.pipe.skipped
  $candc/bin/generate -g $candc/src/data/ccg/cats $candc/src/data/ccg/cats/markedup reference.pipe.skipped >reference.grs.skipped
  $candc/bin/generate -g $candc/src/data/ccg/cats $candc/src/data/ccg/cats/markedup output.pipe.skipped >output.grs.skipped
  python formatgrs.py reference.grs.skipped >reference.grs.format
  python formatgrs.py output.grs.skipped >output.grs.format
  $candc/working/evaluate/evalParse.pl reference.grs.format output.grs.format >>log.$i
  # ;
  rm -r scratch
  $zpar/scripts/ccg/evaluate.zpar.sh output reference
  cp output.txt output.$i.txt
  #cp test_db test_db.$i
  #gzip test_db.$i
done

