#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t c_pid, pid;
    int status;
    int i;
    int numforks=0;

    for(i=0; i<4 ; i++){  
        c_pid = fork();   //duplicate the parent.
        if(c_pid == 0){   //child code breaks. parent code continues.
            break;
        }
        numforks++;       // remember the number of child processes created.
    }
    if( c_pid == 0 ){    //child
        pid = getpid();
        printf("Child: %d: I'm the child %d\n", pid, c_pid);
        printf("Child: sleeping for 2-seconds, then exiting with status 12\n");
        sleep(2);        //sleep for 2 seconds
        exit(12);        //exit with status 12
    }else if (c_pid > 0){//parent
        while(numforks){ //waiting for child to terminate
            pid = wait(&status);
            if ( WIFEXITED(status) ){
                printf("Parent: Child exited with status: %d\n",
                                           WEXITSTATUS(status));
            }
            numforks--;
        }
    }else{        //error: The return of fork() is negative
        perror("fork failed");
        _exit(2); //exit failure, hard
    }
    return 0; //success
}

