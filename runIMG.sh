#!/bin/bash
#
#BSUB -J IMAGE
#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -q normal
#BSUB -e %J.error
#BSUB -o %J.output
 
 ./img -g image.bin -b 5.0 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a SMKDETACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a SMKDETACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a SMKRANACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a SMKRANACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG4ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG4ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG4ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG4ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG4ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG4ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG4ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a ALG3ACC >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a ALG3ACC >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a SMKSTREAM >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a SMKSTREAM >>out_fb_img.csv;
 
  ./img -g image.bin -b 5.0 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a SAMPLEGREEDY >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a SAMPLEGREEDY >>out_fb_img.csv;
 
 ./img -g image.bin -b 5.0 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 4.5 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 4.0 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 3.5 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 3.0 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 2.5 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 2.0 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 1.5 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 1.0 -a FANTOM >>out_fb_img.csv;
 ./img -g image.bin -b 0.5 -a FANTOM >>out_fb_img.csv;
 
