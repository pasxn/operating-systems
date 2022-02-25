#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// for exec
char* ls_args[4] = {NULL, NULL, NULL, NULL};

void split_text(char* input) {

    char * token = strtok(input, " ");
    int i = 0;
    while( token != NULL ) {
        ls_args[i] = token;
        // printf("%s\n", token); 
        token = strtok(NULL, " ");
        i++;
    }
}

int main() {
    pid_t c_pid, pid;
    char ch;
    int status;
    char text[100];
    int i=0;
    int numforks=0;

    printf("> ");
    while((ch=getchar())!=EOF) {
        switch(ch) {
            case '\n':
                text[i]='\0';
     
                if (strcmp(text, "exit") == 0) {
                    exit(0);
                }
                
                c_pid = fork();   
                numforks++;     
                i=0;

                if(c_pid == 0) {
                    // printf("User entered text: %s\n", text);
                    split_text(text);
                    execvp( ls_args[0], ls_args);
                    printf("execve failed: No such command!");
                    // sleep(5);        
                    _exit(2);      
                    break;
                } else if (c_pid > 0) {
                    while(numforks) { 
                        pid = wait(&status);
                        if ( WIFEXITED(status) ) {
                            // printf("Parent: Child exited with status: %d\n",WEXITSTATUS(status));
                        }
                        printf("> ");
                        numforks--;
                    }
                } else {        
                    perror("fork failed");
                    _exit(2); 
                }    
                break;

            default:
                text[i]=ch;
                i++;
        }
    }
    printf("\n");
    return 0; 
}