#!/usr/bin/env python3

from concurrent import futures as f
import time

def fun_with_arg(arg):
    print("This is sleeping:", arg)
    time.sleep(arg)

    return arg**2

if __name__ == '__main__':

    with f.ThreadPoolExecutor() as exec:
        results = [exec.submit(fun_with_arg, i) for i in range(1, 6)]
        
        for j in f.as_completed(results):
            print(j.result())