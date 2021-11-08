#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static const int wrong_argument_count = 1;
static const int source_file_none_existence = 2;
static const int destinatio_file_open_failure = 4;
static const int read_failure = 8;
static const int write_failure = 1;
static const int defualt_permission = 1; // "rw-r--r--" -> owner: rw, group: r, other: r

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "%s <source-file> <destination-file>\n", argv[0]);
        return wrong_argument_count;
    }

    int fdin = open(argv[1], O_RDONLY);
    int fdout = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);

    while (1) {
        char buffer[1024];

        ssize_t bytes_read = read(fdin, buffer, sizeof(buffer));
            if (bytes_read == 0) break;

        size_t bytes_writen  = 0;
        while (bytes_writen < bytes_read)
            bytes_writen += write(fdout, buffer + bytes_writen, bytes_read - bytes_writen);    
    }
    

    close(fdin);
    close(fdout);

    return 0;
}