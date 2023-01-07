set terminal pdf font "Arial,20"
set output 'FCB_F.pdf'
set datafile separator ','

set title 'Facebook (n=4039)' font 'Arial-Bold, 20'

set xrange [0:6]
set xtics (1,2,3,4,5)
set yrange [800:4500]

set style data linespoints
set style line 1  linetype 2 linecolor rgb '#003399' linewidth 2 pointtype 4 pointsize 1.5 pointinterval 0
set style line 2  linetype 2 linecolor rgb "#BB2F3C"  linewidth 2 pointtype 89 pointsize 1.5 pointinterval 0

set style line 3  linetype 2 linecolor rgb "#FFD061"  linewidth 2 pointtype 12 pointsize 1.5 pointinterval 0
set style line 4  linetype 2 linecolor rgb "#D9C68E"  linewidth 2 pointtype 10 pointsize 1.5 pointinterval 0
set style line 5  linetype 2 linecolor rgb "#B1C97F"  linewidth 2 pointtype 81 pointsize 1.5 pointinterval 0


set grid

bm = 0.15
lm = 0.15
rm = 0.85
gap = 0.05
size = 0.70
#kk = 0.3 # relative height of bottom plot
y1 = 5; y2= 8

#set multiplot
#set border 1+2+8
set xtics nomirror
set ytics nomirror

set lmargin at screen lm
set rmargin at screen rm
set bmargin at screen bm
set tmargin at screen bm + size-0.05

#set yrange [y1:y2]
set ytics ('1K' 1000,'2K' 2000,'3K' 8000,'4' 4000)

set xlabel 'B' font 'Arial-Bold, 20' offset 0,1
set ylabel 'Objective value' font 'Arial-Bold, 18' offset 1,0

set key horiz
set key left top font ",11"
plot "out1.csv" using 1:3 linestyle 1 title 'FSA',\
	"out1.csv" using 1:7 linestyle 2 title 'IFSA',\
	"out1.csv" using 1:19 linestyle 3 title 'DS',\
	"out1.csv" using 1:31 linestyle 4 title 'RS',\
	"out1.csv" using 1:43 linestyle 5 title 'Greedy'
