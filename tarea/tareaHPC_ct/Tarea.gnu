Tarea.gnu


set terminal postscript enhanced eps color solid "Times-Roman" 20
set output "./grafico.eps"
set title "n Vs Tiempo"
set xlabel "n"
set ylabel "Time (microsecs)"
set style func linespoints
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0
set key left
plot [] [] \
	'./results/QSA' using 1:3 title "QuickSort-A" with linespoints lt -1 pt 7 lw 0.5, \
	'./results/QSB' using 1:3 title "QuickSort-B" with linespoints lt -1 pt 3 lw 0.5 lc rgb '#DC143C',\

