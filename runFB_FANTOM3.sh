#!/bin/bash
#
#BSUB -J FB
#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -q normal
#BSUB -e %J.error
#BSUB -o %J.output
 
 ./revmax_nm -g fb.bin -b 3.0 -a FANTOM >>out_fb_fantom3.csv;