#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void main() {
  printf("Line 1\n");
  getchar();

  fork();

  printf("Line 2\n");
  getchar();
}
