#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
                    printf("User entered text: %s\n", text);
                    sleep(5);        
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