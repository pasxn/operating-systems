#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

static int my_system(const  char* command) {
    pid_t pid = fork();
    if(pid == 0) {
        char* arguments[] = {"/bin/sh", "-c", (char*) command, NULL};
        execvp(arguments[0] ,arguments);
        printf("Failed to invoke\n");
        exit(0);
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -WTERMSIG(status);
}

static const size_t k_max_line = 2048;

int main(int argc, char** argv) {
    char command[k_max_line];
    while(true) {
        printf("> ");
        fgets(command, k_max_line, stdin);
        
        if(feof(stdin))
            break;

        command[strlen(command) - 1] = '\0';
        printf("Return code: %d\n", my_system(command));
    }
    printf("\n");
    return 0;
}