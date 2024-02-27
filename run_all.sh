#!/bin/bash

function run_one() {
    printf "##### run %s %s #####\n" "$1" "$2"
    cd "$1" && "$2" && cd ..
    printf "#####################\n\n"
}

run_one ./getpid ./run.sh
run_one ./getpid_multithread ./run.sh

