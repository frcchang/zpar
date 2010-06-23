rm log.sentlen.lp*
rm log.sentlen.lr*
rm log.sentlen.lf*
for index in 1 11 21 31 41 ; do
  upper=$(($index + 9))
  python filtersentlen.py devr.ccgbank_deps devr.stagged $index $upper >tmp0
  python filtercategorylen.py devr.stagged $index $upper >tmp0.stagged
  python filtersentlen.py devo.candc devr.stagged $index $upper >tmp1
  python filtersentlen.py devo.exc.candc devr.stagged $index $upper >tmp2
  ../../../../../app/candc/src/scripts/ccg/evaluate tmp0.stagged tmp0 tmp1 >tmp1.log
  grep lp tmp1.log | cut -d\  -f5 | cut -d% -f1 >>log.sentlen.lp_candc
  grep lr tmp1.log | cut -d\  -f5 | cut -d% -f1 >>log.sentlen.lr_candc
  grep lf tmp1.log | cut -d\  -f5 | cut -d% -f1 >>log.sentlen.lf_candc
  ../../../../../app/candc/src/scripts/ccg/evaluate tmp0.stagged tmp0 tmp2 >tmp2.log
  grep lp tmp2.log | cut -d\  -f5 | cut -d% -f1 >>log.sentlen.lp_ours
  grep lr tmp2.log | cut -d\  -f5 | cut -d% -f1 >>log.sentlen.lr_ours
  grep lf tmp2.log | cut -d\  -f5 | cut -d% -f1 >>log.sentlen.lf_ours
done  
echo '#candc	ours' >> log.sentlen.lp
echo '#candc	ours' >> log.sentlen.lr
echo '#candc	ours' >> log.sentlen.lf
paste log.sentlen.lp_candc log.sentlen.lp_ours >>log.sentlen.lp
paste log.sentlen.lr_candc log.sentlen.lr_ours >>log.sentlen.lr
paste log.sentlen.lf_candc log.sentlen.lf_ours >>log.sentlen.lf
rm tmp0
rm tmp0.stagged
rm tmp1
rm tmp2
rm tmp1.log
rm tmp2.log
