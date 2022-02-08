#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

static const char* str = "abcd";

int main(int argc, char** argv) {
    for(size_t i = 0; i<strlen(str); i++) {
        printf("%c\n", str[i]);
        pid_t pid = fork();
        assert(pid >= 0);
    }
}