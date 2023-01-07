## Linear-Time and Deterministic Algorithms for Cardinality-Constrained Non-Monotone Submodular Maximization

The implementations of all algorithms evaluated in the paper,
for both `revmax` and `maxcut` applications are in `src/algs.cpp`.

### Dependencies
- GNU `g++`, `make`
- `python3` with `matplotlib` for plotting figures

### Reproduce Experiments in Paper
To reproduce all figures in the paper:
```
make single-pass
```
Results will be plotted in the `exp/figs` directory.

