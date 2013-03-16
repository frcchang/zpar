segmentor=zpar/dist/segmentor

rm model

for i in `seq 1 30`;
do
$segmentor/train train.txt model 1
$segmentor/segmentor model input.txt output.txt 
python evaluate.py output.txt reference.txt
cp model model.$i
done
