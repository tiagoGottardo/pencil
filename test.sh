#!/bin/bash

START=$(date +%s.%N)
g++ -DTEST_MODE ./tests/*.cpp ./tests/Tester.h -pthread -o test 
./test
END=$(date +%s.%N)

DURATION=$(echo "$END - $START" | bc)
echo "Execution time: ${DURATION} seconds"

rm test 
