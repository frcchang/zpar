rm log.deplen.lp*
rm log.deplen.lr*
rm log.deplen.lf*
for index in `seq 1 30`; do
  python filterdeplen.py devr.ccgbank_deps $index $index >tmp0
  python filterdeplen.py devo.candc $index $index >tmp1
  python filterdeplen.py devo.exc.candc $index $index >tmp2
  ../../../../../app/candc/src/scripts/ccg/evaluate devr.stagged tmp0 tmp1 >tmp1.log
  grep lp tmp1.log | cut -d\  -f5 | cut -d% -f1 >>log.deplen.lp_candc
  grep lr tmp1.log | cut -d\  -f5 | cut -d% -f1 >>log.deplen.lr_candc
  grep lf tmp1.log | cut -d\  -f5 | cut -d% -f1 >>log.deplen.lf_candc
  ../../../../../app/candc/src/scripts/ccg/evaluate devr.stagged tmp0 tmp2 >tmp2.log
  grep lp tmp2.log | cut -d\  -f5 | cut -d% -f1 >>log.deplen.lp_ours
  grep lr tmp2.log | cut -d\  -f5 | cut -d% -f1 >>log.deplen.lr_ours
  grep lf tmp2.log | cut -d\  -f5 | cut -d% -f1 >>log.deplen.lf_ours
done  
echo '#candc	ours' >> log.deplen.lp
echo '#candc	ours' >> log.deplen.lr
echo '#candc	ours' >> log.deplen.lf
paste log.deplen.lp_candc log.deplen.lp_ours >>log.deplen.lp
paste log.deplen.lr_candc log.deplen.lr_ours >>log.deplen.lr
paste log.deplen.lf_candc log.deplen.lf_ours >>log.deplen.lf
rm tmp0
rm tmp1
rm tmp2
rm tmp1.log
rm tmp2.log
