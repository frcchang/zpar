# $1 data folder containing t[1-10].xml and tp[1-82].xml; $2 target file; $3 opt
rm $2.train
rm $2.test
for i in `seq 1 82`; do
#  python dgaxml2dep.py -u $1/tp$i.xml >>$2.train
  python dgaxml2dep.py $1/tp$i.xml >>$2.train
done
for i in `seq 1 10`; do
#  python dgaxml2dep.py -u $1/t$i.xml >>$2.test
  python dgaxml2dep.py $1/t$i.xml >>$2.test
done
