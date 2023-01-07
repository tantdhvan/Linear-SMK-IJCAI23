set terminal pdf font "Arial,20"
set output 'FB_F.pdf'
set datafile separator ','

set title 'Facebook (n=4039)' font 'Arial-Bold, 20'

set xtics  norangelimit 
set xtics   ()
set yrange [2000:10000]
set ytics ('2K' 2000,'4K' 4000, '6K' 6000, '8K' 8000)

set style data histograms
set style histogram clustered gap 3
set grid

bm = 0.2
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
set tmargin at screen bm + size - 0.05

set xlabel 'B (% total cost)' font 'Arial-Bold, 20' offset 0,0.5
set ylabel 'Objective value' font 'Arial-Bold, 18' offset 0.5,0

set key horiz
set key left top font ",11" spacing 1 width 2
plot "out1.csv" using 55:xtic(1) title 'GREEDY' fillstyle pattern 5 lt rgb "#7EA157",\
"out1.csv" using 51:xtic(1) title 'FANTOM' fillstyle pattern 0 lt rgb "#C9643C",\
"out1.csv" using 31:xtic(1) title 'SMKDETACC' fillstyle pattern 1 lt rgb "#FFD061",\
"out1.csv" using 47:xtic(1) title 'SMKSTREAM' fillstyle pattern 2 lt rgb "#B1C97F",\
"out1.csv" using 43:xtic(1) title 'SMKRANACC' fs pattern 4 lt rgb "#D9C68E",\
"out1.csv" using 19:xtic(1) title 'RanLinApp' fillstyle pattern 3 lt rgb "#BB2F3C",\
"out1.csv" using 7:xtic(1) title 'DetLinApp' fillstyle pattern 6 lt rgb "#003399"