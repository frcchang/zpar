zpar=./zpar/dist/english.postagger

rm model

for i in `seq 1 10`;
do
$zpar/train train.txt model 1
$zpar/tagger input.txt output.txt model 
python evaluate.py output.txt reference.txt
cp model model.$i
gzip model.$i
done
