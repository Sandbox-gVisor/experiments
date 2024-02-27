# Experiment info

In `main.c` there is a program that consumes 4 arguments
- `num_threads`
- `num_experiments`
- `num_iterations` in one experiment
- `filename` to store results (like .csv but separator is ';')

Program starts `num_threads` child threads, which do all `num_experiments` experiments. Main thread is waiting for all children to complete.

For each experiment the work is done. In this program work is calling `getpid()` `num_iterations` times.

Some results will be printed to `stdout`. All results will be written to file with name `filename`.