#!/usr/bin/env python3

import threading
import time

def fun_with_arg(arg):
    print("This is sleeping:", arg)
    time.sleep(arg)

def do_something():
    print("This is do something")
    time.sleep(2)

def task(param):
    for i in range(5):
        print("This is a task with:", param)
        time.sleep(2)

if __name__ == '__main__':
    
    t1 = threading.Thread(target = fun_with_arg, args = [0.5])
    t2 = threading.Thread(target = fun_with_arg, args = [1.5])
    
    t1.start()
    t2.start()

    # waiting until the threads are finished
    t1.join()
    print("T1 Done!!!")

    t2.join()
    print("T2 Done!!!")