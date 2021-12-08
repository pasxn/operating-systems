#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    char *cmd = "ls";
    char *argv[3];
    argv[0] = "ls";
    argv[1] = "-la";
    argv[2] = NULL;
    execvp(cmd, argv); 
}