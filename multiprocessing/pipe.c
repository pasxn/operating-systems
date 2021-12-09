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
        char buffer[6];
        read(fds[0], buffer, sizeof(buffer));
        
        printf("From parent through pipe: %s\n", buffer);
        close(fds[0]); // done reading

        return 0;
    }

    // parent
    close(fds[0]); // no need of reading;
    write(fds[1], "Hello", 6);
    waitpid(pid, NULL, 0);
    close(fds[1]); // done

    return 0;
}