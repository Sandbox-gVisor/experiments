#!/bin/bash

function run_with_num_treads() {
    printf "\n*** run with %d thread data in %s ***\n" "$1" "$2"
    ./a.out "$1" 30 100000 "$2"
    printf "*******************\n"
}

gcc main.c
run_with_num_treads 1 out_1.txt
run_with_num_treads 2 out_2.txt
run_with_num_treads 4 out_4.txt
run_with_num_treads 8 out_8.txt
run_with_num_treads 16 out_16.txt
printf "\n"
