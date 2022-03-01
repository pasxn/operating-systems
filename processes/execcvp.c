#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[]){
    //arguments for ls, will run: ls -l /bin
    char * ls_args[4] = { "ls", "-l", "/bin", NULL} ;
 
    //execute ls
    execvp( ls_args[0], ls_args);
    
    //only get here if exec failed
    perror("execvp failed");
    
    return 2; //return error status
}