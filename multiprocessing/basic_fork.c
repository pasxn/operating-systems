#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    printf("Greetings from process %d! My parent is %d\n", getpid(), getppid());
    pid_t pid = fork();
    if(pid > 0)
        printf("This is the parent process %d! My parent is %d. I created the child process %d\n", getpid(), getppid(), pid);
    else
        printf("This is the child process %d! I got %d from fork. My parent is %d\n", getpid(), pid, getppid());
}