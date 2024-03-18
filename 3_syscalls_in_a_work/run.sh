#!/bin/bash

gcc main.c

echo "sleeping 20 sec..."
sleep 20

./a.out 30 10000 out.txt
