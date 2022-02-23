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
     
                if (strcpy(text, "exit") == 0) {
                    exit(0);
                }
                
                c_pid = fork();   

                if(c_pid == 0) {
                    break;
                }    
                
                numforks++;     
                i=0;
                printf("> ");

                break;
            default:
                text[i]=ch;
                i++;
        }
    }
    if( c_pid == 0 ) {    
        printf("Child\n");
        printf("User entered text: %s\n", text);
        // sleep(5);        
        _exit(2);        
    }else if (c_pid > 0) {
        printf("Parent\n");
        while(numforks) { 
            pid = wait(&status);
            if ( WIFEXITED(status) ) {
                printf("Parent: Child exited with status: %d\n",
                                           WEXITSTATUS(status));
            }
            numforks--;
        }
    }else {        
        perror("fork failed");
        _exit(2); 
    }
    return 0; 
}