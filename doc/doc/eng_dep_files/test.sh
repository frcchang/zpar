zpar=./zpar/dist/english.depparser

rm model

for i in `seq 1 30`;
do
$zpar/train train.txt model 1
$zpar/depparser input.txt output.txt model 
python evaluate.py output.txt reference.txt
cp model model.$i
gzip model.$i
done
