#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

const char* k_file_name = "my_file.txt";
const int k_file_exists_err = 17;

int main() {
    umask(0);
    int file_descripter = open(k_file_name, O_WRONLY | O_CREAT | O_EXCL, 0644);

    if(file_descripter == -1) {
        printf("There was a problem creating '%s'!\n", k_file_name);

        if (errno == k_file_exists_err)
            printf("The file already exists!\n");
        else
            printf("unknown errno: %d\n", errno);

        return -1;
    }

    close(file_descripter);
}