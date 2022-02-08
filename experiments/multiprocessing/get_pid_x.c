#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    pid_t pid = getpid();
    printf("PID: %5d\n", pid);
}