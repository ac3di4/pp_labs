#!/bin/bash
# please run this script from lab4/code directory

# compile
gcc main.c -fopenmp -o main

# static
echo running static
OMP_SCHEDULE=static ./main > ../data/static.csv
echo 10
OMP_SCHEDULE=static,10 ./main > ../data/static_10.csv
echo 10000
OMP_SCHEDULE=static,10000 ./main > ../data/static_10000.csv
echo 10000000
OMP_SCHEDULE=static,10000000 ./main > ../data/static_10000000.csv

# dynamic
echo running dynamic
OMP_SCHEDULE=dynamic ./main > ../data/dynamic.csv
echo 10
OMP_SCHEDULE=dynamic,10 ./main > ../data/dynamic_10.csv
echo 10000
OMP_SCHEDULE=dynamic,10000 ./main > ../data/dynamic_10000.csv
echo 10000000
OMP_SCHEDULE=dynamic,10000000 ./main > ../data/dynamic_10000000.csv

# guided
echo running guided
OMP_SCHEDULE=guided ./main > ../data/guided.csv
echo 10
OMP_SCHEDULE=guided,10 ./main > ../data/guided_10.csv
echo 10000
OMP_SCHEDULE=guided,10000 ./main > ../data/guided_10000.csv
echo 10000000
OMP_SCHEDULE=guided,10000000 ./main > ../data/guided_10000000.csv

# auto
echo running auto
OMP_SCHEDULE=auto ./main > ../data/auto.csv
