#!/bin/bash
#
#BSUB -J FB_MC
#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -q gpu
#BSUB -e %J.error
#BSUB -o %J.output
 
 ./maxcut -g fb.bin -b 0.02 -a SMKDETACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a SMKDETACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a SMKDETACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a SMKDETACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a SMKDETACC >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a SMKDETACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a SMKRANACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a SMKRANACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a SMKRANACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a SMKRANACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a SMKRANACC >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a SMKRANACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG4ACC >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a ALG4ACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG4ACC >>out_fb.csv;
   ./maxcut -g fb.bin -b 0.12 -a ALG4ACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG4ACC >>out_fb.csv;
   ./maxcut -g fb.bin -b 0.12 -a ALG4ACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG4ACC >>out_fb.csv;
   ./maxcut -g fb.bin -b 0.12 -a ALG4ACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG4ACC >>out_fb.csv;
   ./maxcut -g fb.bin -b 0.12 -a ALG4ACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG4ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG4ACC >>out_fb.csv;
   ./maxcut -g fb.bin -b 0.12 -a ALG4ACC >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a ALG3ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a ALG3ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a ALG3ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a ALG3ACC >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a ALG3ACC >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a ALG3ACC >>out_fb.csv;
 
  ./maxcut -g fb.bin -b 0.02 -a SAMPLEGREEDY >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a SAMPLEGREEDY >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a SAMPLEGREEDY >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a SAMPLEGREEDY >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a SAMPLEGREEDY >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a SAMPLEGREEDY >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a SMKSTREAM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a SMKSTREAM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a SMKSTREAM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a SMKSTREAM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a SMKSTREAM >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a SMKSTREAM >>out_fb.csv;
 
 ./maxcut -g fb.bin -b 0.02 -a FANTOM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.04 -a FANTOM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.06 -a FANTOM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.08 -a FANTOM >>out_fb.csv;
 ./maxcut -g fb.bin -b 0.1 -a FANTOM >>out_fb.csv;
  ./maxcut -g fb.bin -b 0.12 -a FANTOM >>out_fb.csv;

 
