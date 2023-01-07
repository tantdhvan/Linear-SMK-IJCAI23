set terminal pdf font "Arial,20"
set output 'FB_Q.pdf'
set datafile separator ','

set title 'Facebook (n=4039)' font 'Arial-Bold, 20'

set xrange [1:13]
set xtics (2,4,6,8,10,12)
set format x "%g%%"
set yrange [8:15]
set ytics ('10K' 9.2103,'30K' 10.309,'10M' 11.1181, '100M' 12.4207)

set style data linespoints
set style line 1  linetype 2 linecolor rgb "#003399"  linewidth 2 pointtype 4 pointsize 1 pointinterval 0
set style line 2  linetype 2 linecolor rgb "#BB2F3C"  linewidth 2 pointtype 89 pointsize 1 pointinterval 0
set style line 3  linetype 2 linecolor rgb "#FFD061"  linewidth 2 pointtype 12 pointsize 1 pointinterval 0
set style line 4  linetype 2 linecolor rgb "#D9C68E"  linewidth 2 pointtype 10 pointsize 1 pointinterval 0
set style line 5  linetype 2 linecolor rgb "#B1C97F"  linewidth 2 pointtype 81 pointsize 1 pointinterval 0
set style line 6  linetype 2 linecolor rgb "#C9643C"  linewidth 2 pointtype 75 pointsize 1 pointinterval 0
set style line 7  linetype 2 linecolor rgb "#7EA157"  linewidth 2 pointtype 76 pointsize 1 pointinterval 0
set style line 8  linetype 2 linecolor rgb "#FDEA8B"  linewidth 2 pointtype 77 pointsize 1 pointinterval 0



set grid

bm = 0.2
lm = 0.2
rm = 0.85
gap = 0.05
size = 0.70
#kk = 0.3 # relative height of bottom plot
y1 = 8; y2= 15

#set multiplot
#set border 1+2+8
set xtics nomirror
set ytics nomirror

set lmargin at screen bm
set rmargin at screen rm
set bmargin at screen bm
set tmargin at screen bm + size-0.05

set xlabel 'B (% total cost)' font 'Arial-Bold, 20' offset 0,0.5 
set ylabel 'Number of queries' font 'Arial-Bold, 18' offset 0.5,0

set key horiz
set key left top font ",11" spacing 1 width 2
plot "out1.csv" using 1:8 linestyle 2 title 'DetLinApp',"out1.csv" using 1:20 linestyle 3 title 'RanLinApp',"out1.csv" using 1:32 linestyle 4 title 'SMKDETACC',"out1.csv" using 1:44 linestyle 5 title 'SMKRANACC',"out1.csv" using 1:48 linestyle 6 title 'SMKSTREAM',"out1.csv" using 1:52 linestyle 7 title 'FANTOM',"out1.csv" using 1:56 linestyle 8 title 'GREEDY'
