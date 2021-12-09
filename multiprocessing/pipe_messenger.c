#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    int fds[2]; // 0:reader, 1:writer
    pipe(fds);

    pid_t pid = fork();
    if(pid == 0) { // child
        // child will read from the pipe, no need of writing
        close(fds[1]);
        char* input;
        while(input != 0) {
            read(fds[0], input, sizeof(input));
            printf("From parent through pipe: %d\n", ((int)(*input))*2);
        }
        close(fds[0]); // done reading
        return 0;
    }

    // parent
    close(fds[0]); // no need of reading;
    
    char* input;
    while(input != 0) {
        printf("> ");
        scanf ("%c",&*input);

        write(fds[1], input, sizeof(input));
        waitpid(pid, NULL, 0);
    }
    
    close(fds[1]); // done

    return 0;
}