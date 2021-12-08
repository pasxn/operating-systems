#include <unistd.h>

int main() {
    char* argv[3];
    argv[0] = "ls";
    argv[1] = "-la";
    argv[2] = NULL;
    execvp(argv[0], argv); 
}