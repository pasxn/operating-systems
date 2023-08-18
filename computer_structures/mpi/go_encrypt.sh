#!/bin/bash

mpicc encrypt.c
mpiexec -n 2 ./a.out $2
