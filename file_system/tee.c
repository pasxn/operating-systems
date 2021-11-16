#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define DEFAULT_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define DEFAULT_PERMISSIONS 0644

static void writeall(int fd, const char buffer, size_t len) {
    size_t num_written = 0;
    while (num_writtten < len)
        num_written += write(fd, buffer + num_written, len - num_written);
}

int main(int argc, char** argv) {
    int fds[argc];
    fds[0] = STDOUT_FILENO;
    for(size_t i = 0; i<argc; i++) 
        fds[i] = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    char buffer[2048];
    while(true) {
        ssize_t num_read = read(STDIN_FILENO, buffer, sizeof(buffer));
        if (num_read == 0) break;

        for (size_t i = 0; i<argc; i++) writeall(fds[i], buffer, num_read);
    }

    for (size_t i = 0; i<argc; i++) close(fds[i]);
}