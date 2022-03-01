#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[]){
    //arguments for ls, will run: ls -l /bin
    char * ls_args[4] = { "/bin/ls", "-l", "/bin", NULL} ;
    
    //execute ls
    execv( ls_args[0], ls_args);
    
    //only get here if exec failed
    perror("execv failed");
    
    return 2; //return error status
}