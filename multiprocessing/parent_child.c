#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    srandom(time(NULL));
    printf("I only get printed once\n");
    
    pid_t pid = fork();
    bool parent = (pid != 0);
    if( (random() % 2 == 0) == parent )
        sleep(2);

    if(parent)
        waitpid(pid, NULL, 0);

    printf("I get printed twice (This one is from %s)\n", parent ? "parent" : "chid");
}