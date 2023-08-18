#!/bin/bash

gcc $1 -pthread
DEBUG=$4 ./a.out $2 $3
