## Linear Query Approximation Algorithms for Non-monotone Submodular Maximization under Knapsack Constraint

The implementations of all algorithms evaluated in the paper,
for both `revmax`, `image` and `maxcut` applications are in `src/algs.cpp`.

### Dependencies
- GNU `g++`, `make`
- `python3` with `matplotlib` for plotting figures

### The source code is organized as follows:

algs.cpp: contains experimental algorithms, including Alg3, Alg4, SmkDetAcc, SmkRanAcc, SmkStream, Fantom, and SampleGreedy.
obj.cpp: stores functions for calculating function f.
binheap.cpp: implements a heap structure.
mygraph.cpp: includes the data structure.
gen_er.cpp: used for generating ER datasets.

### To run the source code, follow these steps:
Step 1: Build the makefile by running "make" in the project directory.
Step 2: Test the application using the following commands:

Image summarization:
./img -g data.bin -b B -a alg
Revenue Maximization:
./revmax_nm -g data.bin -b B -a alg
Maxcut:
./maxcut -g data.bin -b B -a alg
Where:
data.bin is the dataset.
B is the budget as a percentage of total cost.
alg is the algorithm to be used.

