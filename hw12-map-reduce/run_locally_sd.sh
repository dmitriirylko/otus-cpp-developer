#!/bin/sh
BINARY_HOME=./bin
INPUT_HOME=./input
INPUT=${INPUT_HOME}/AB_NYC_2019.csv

make clean
make all
cat ${INPUT} | ${BINARY_HOME}/mapper_sd | sort -k1 | ${BINARY_HOME}/reducer_sd > output_sd
cat output_sd