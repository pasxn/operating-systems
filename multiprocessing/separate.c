#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    printf("Before\n");
    pid_t pid = fork();
    printf("after\n");
    
    if(pid == 0) { // child 
        printf("I am the child. parent will wait for me!\n");
        return 69;
    }
    // parent
    int status;
    waitpid(pid, &status, 0);

    if(WIFEXITED(status)) {
        printf("child exited with status %d\n", WEXITSTATUS(status));
    } else {
        printf("child terminated abnormally\n");
    }
}