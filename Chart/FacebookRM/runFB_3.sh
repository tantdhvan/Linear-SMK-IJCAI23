#!/bin/bash
#
#BSUB -J FB2_RM
#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -q normal
#BSUB -e %J.error
#BSUB -o %J.output
 
 ./revmax_nm -g fb.bin -b 40 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a SMKDETACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a SMKRANACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40-a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a ALG3ACC >>out_fb_rm_2.csv;
 
  ./revmax_nm -g fb.bin -b 40 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a SMKSTREAM >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 40 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 50 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 60 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 70 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 80 -a FANTOM >>out_fb_rm_2.csv;

 
