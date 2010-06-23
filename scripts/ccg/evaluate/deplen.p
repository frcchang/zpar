set output 'deplen.lp.eps';
set xlabel "dependency length"
set ylabel "precision"
set title "Precision comparison by dependency length"
set xrange [1:30];
#set yrange [0.4:0.9];
set key bottom;
#      set   autoscale                        # scale axes automatically
#      unset log                              # remove any log-scaling
#      unset label                            # remove any previous labels
#      set xtic auto                          # set xtics automatically
#      set ytic auto                          # set ytics automatically
plot    "log.deplen.lp" using 1 title 'ours' with linespoints, "log.deplen.lp" using 2 title 'C&C' with linespoints

