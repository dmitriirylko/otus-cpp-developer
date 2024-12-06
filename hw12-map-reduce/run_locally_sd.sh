#!/bin/sh
BINARY_HOME=./bin
INPUT_HOME=./input
INPUT=${INPUT_HOME}/AB_NYC_2019.csv

cat ${INPUT} | ${BINARY_HOME}/mapper_sd "$(cat output_mean)" | ${BINARY_HOME}/reducer_sd > output_sd
cat output_sd