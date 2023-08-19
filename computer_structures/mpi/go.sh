#!/bin/bash

num_cpus=$(nproc)
mpicc $1
mpiexec -n $num_cpus ./a.out $num_cpus $2
