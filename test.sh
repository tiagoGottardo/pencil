#!/bin/bash

g++ -DTEST_MODE ./tests/*.cpp ./tests/Tester.h -o test 
./test
rm test 
