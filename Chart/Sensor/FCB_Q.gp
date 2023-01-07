set terminal pdf font "Arial,14"
set output 'SS_Q.pdf'
set datafile separator ','

set title 'Intel Lab Sensor data (n=56)' font 'Arial-Bold, 18'

set xrange [8:52]
set xtics (10,20,30,40,50)
set yrange [0:25]


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
y1 = 0; y2= 25

#set multiplot
#set border 1+2+8
set xtics nomirror
set ytics nomirror

set lmargin at screen lm
set rmargin at screen rm
set bmargin at screen bm
set tmargin at screen bm + size-0.05
#set yrange [y1:y2]
set ytics ('150' 5, '65.7M' 18)

set xlabel 'B' font 'Arial-Bold,18' offset 0,0.6

set ylabel 'Number of queries' font 'Arial-Bold,18' offset 0.8,0

set key horiz
set key left top font ",11"
plot "out1.csv" using 1:4 linestyle 1 title 'FSA',\
	"out1.csv" using 1:8 linestyle 2 title 'IFSA',\
	"out1.csv" using 1:20 linestyle 3 title 'DS',\
	"out1.csv" using 1:32 linestyle 4 title 'RS',\
	"out1.csv" using 1:44 linestyle 5 title 'Greedy'
