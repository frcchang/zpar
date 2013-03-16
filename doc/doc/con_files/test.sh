parser=zpar/dist/english.conparser

rm model

for i in `seq 1 30`;
do
$parser/train train.txt model 1
$parser/conparser model input.txt output.txt 
./evalb -p COLLISN.prm output.txt reference.txt
cp model model.$i
done
