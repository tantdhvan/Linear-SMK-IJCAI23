#!/bin/bash
#
#BSUB -J ENRON
#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -q normal
#BSUB -e %J.error
#BSUB -o %J.output
 
  
./revmax_nm -g enron.bin -b 0.02 -a alg3acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a alg3acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a alg3acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a alg3acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a alg3acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a alg3acc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a alg4acc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a alg4acc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a alg4acc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a alg4acc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a alg4acc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a alg4acc >>out_e.csv
 
./revmax_nm -g enron.bin -b 0.02 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a smkranacc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a smkranacc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a smkranacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a smkranacc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a smkdetacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a smkdetacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a smkdetacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a smkdetacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a smkdetacc >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a smkdetacc >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a samplegreedy >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a samplegreedy >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a samplegreedy >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a samplegreedy >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a samplegreedy >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a samplegreedy >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a smkstream >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a smkstream >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a smkstream >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a smkstream >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a smkstream >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a smkstream >>out_e.csv

./revmax_nm -g enron.bin -b 0.02 -a fantom >>out_e.csv
./revmax_nm -g enron.bin -b 0.04 -a fantom >>out_e.csv
./revmax_nm -g enron.bin -b 0.06 -a fantom >>out_e.csv
./revmax_nm -g enron.bin -b 0.08 -a fantom >>out_e.csv
./revmax_nm -g enron.bin -b 0.10 -a fantom >>out_e.csv
./revmax_nm -g enron.bin -b 0.12 -a fantom >>out_e.csv



