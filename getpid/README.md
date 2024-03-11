# Experiment info

In `main.c` there is a program that consumes 3 arguments
- `num_experiments`
- `num_iterations` in one experiment
- `filename` to store results (like .csv but separator is ';')

For each experiment the work is done. In this program work is calling `getpid()` `num_iterations` times.

Some results will be printed to `stdout`. All results will be written to file with name `filename`.
