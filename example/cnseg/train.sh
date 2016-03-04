#!/bin/sh

../../crf_learn -f 5 -e 0.0001 -p 16 -N 500000 template $1 ${1}.model
