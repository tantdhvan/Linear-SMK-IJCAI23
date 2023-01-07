set terminal pdf font "Arial,20"
set output 'ER_T.pdf'
set datafile separator ','

set title 'ER (n=5000)' font 'Arial-Bold, 20'

set xrange [950:3050]
set xtics ('1K' 1000,'1.5K' 1500,'2K' 2000,'2.5K' 2500,'3K' 3000)
set yrange [1:12]

set style data linespoints
set style line 1  linetype 2 linecolor rgb '#003399' linewidth 2 pointtype 4 pointsize 1.5 pointinterval 0
set style line 2  linetype 2 linecolor rgb "#BB2F3C"  linewidth 2 pointtype 89 pointsize 1.5 pointinterval 0
set style line 3  linetype 2 linecolor rgb "#FFD061"  linewidth 2 pointtype 12 pointsize 1.5 pointinterval 0
set style line 4  linetype 2 linecolor rgb "#D9C68E"  linewidth 2 pointtype 10 pointsize 1.5 pointinterval 0
set style line 5  linetype 2 linecolor rgb "#B1C97F"  linewidth 2 pointtype 81 pointsize 1.5 pointinterval 0
set style line 6  linetype 2 linecolor rgb "#C9643C"  linewidth 2 pointtype 75 pointsize 1.5 pointinterval 0



set grid

bm = 0.2
lm = 0.15
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


#set yrange [y1:y2]
set ytics ('10' 2.3025,'0.5K' 6.214608,'10K' 9.21034)

set xlabel 'B' font 'Arial-Bold, 20' offset 0,0.5
set ylabel 'Running time' font 'Arial-Bold, 18' offset 0.5,0

set key horiz
set key left top font ",11"
plot "out1.csv" using 1:5 linestyle 1 title 'ALG1',\
	"out1.csv" using 1:9 linestyle 2 title 'ALG2',\
	"out1.csv" using 1:21 linestyle 3 title 'ALG3',\
	"out1.csv" using 1:33 linestyle 4 title 'SMKDET',\
	"out1.csv" using 1:45 linestyle 5 title 'SMKRAN',\
	"out1.csv" using 1:49 linestyle 6 title 'SMKSTREAM'