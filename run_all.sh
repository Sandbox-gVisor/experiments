#!/bin/bash

function run_one() {
    printf "##### run %s %s #####\n" "$1" "$2"
    cd "$1" && "$2" && cd ..
    printf "#####################\n\n"
}

echo "sleeping for 20 sec"
sleep 20
echo "start measuring"
run_one ./calc ./run.sh
run_one ./calc_multithread ./run.sh
run_one ./getpid ./run.sh
run_one ./getpid_multithread ./run.sh
