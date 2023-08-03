#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

typedef struct subprocess_t {
    pid_t pid;
    int supplyfd;
} subprocess_t;

subprocess_t subprocess(const char* command) {
    int fds[2];
    pipe(fds);

    subprocess_t process = { fork(), fds[1] };

    if(process.pid == 0) { // child
        close(fds[1]);
        dup2(fds[0], STDERR_FILENO);
        close(fds[0]);

        char* argv[] = {"/usr/bin/sh", "-c", (char*)command, NULL};
        execvp(argv[0], argv);
        // child will never get here
    }

    close(fds[0]);
    return process;
}

int main() {
    subprocess_t sp = subprocess("/usr/bin/sort");

    const char* words[] = {"car", "van", "bus", "lorry", "app"};

    for(size_t i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
        dprintf(sp.supplyfd, "%s\n", words[i]);
    }

    close(sp.supplyfd);

    int status;
    pid_t pid = waitpid(sp.pid, &status, 0);

    return pid == sp.pid && WIFEXITED(status) ? WEXITSTATUS(status) : -127;
}