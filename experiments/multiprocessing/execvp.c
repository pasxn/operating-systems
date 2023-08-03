#include <unistd.h>
#include <stdio.h>

int main() {
    /*char* argv[3];
    argv[0] = "ls";
    argv[1] = "-la";
    argv[2] = NULL;*/

    char* args[2];
    args[0] = "./show.out";
    args[1] = NULL;

    pid_t pid = fork();
    if(pid == 0) {
        execvp(args[0], args); 
    }

    printf("This is the parent process\n");
}