#!/bin/bash

gcc gemm.c -pthread
DEBUG=$3 ./a.out $1 $2
