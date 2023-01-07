#!/bin/bash
#
#BSUB -J FB2_RM
#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -q normal
#BSUB -e %J.error
#BSUB -o %J.output
 
 ./revmax_nm -g fb.bin -b 5 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a SMKDETACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a SMKDETACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a SMKRANACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a SMKRANACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG4ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG4ACC >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a ALG3ACC >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a ALG3ACC >>out_fb_rm_2.csv;
 
  ./revmax_nm -g fb.bin -b 5 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a SAMPLEGREEDY >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a SMKSTREAM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a SMKSTREAM >>out_fb_rm_2.csv;
 
 ./revmax_nm -g fb.bin -b 5 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 10 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 15 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 20 -a FANTOM >>out_fb_rm_2.csv;
 ./revmax_nm -g fb.bin -b 25 -a FANTOM >>out_fb_rm_2.csv;

 
