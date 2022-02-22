#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t c_pid, pid;
    char ch;
    char text[100];
    int i=0;
    int numforks=0;

    while(text != "exit" || ch != EOF) {
        printf(">");
        while((ch=getchar())!=EOF){
            switch(ch){
                case '\n':
                    text[i]='\0';
                    printf("%s\n",text);
                    printf(">");
                    i=0;
                    break;
                default:
                    text[i]=ch;
                    i++;
                    putchar(ch);
            }
        }

        c_pid = fork();   // duplicate the parent
        if(c_pid == 0){   // child code breaks. parent code continues
            break;
            sleep(2);         // sleep for 5 seconds
            _exit(12);        // exit with status 2             
        }  
        numforks++;       // remember the number of child processes created     
    }

    return 0; //success
}

