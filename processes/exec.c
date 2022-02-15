#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char** argv) {
  // arguments for ls, will run: xeyes
  char* argss[4] = {"usr/bin/exeyes", "NULL", "NULL", "NULL"} 
  
  getchar();

  // execute is
  execv(argss[0], argss);

  getchar();

  printf("This line should of never get printed, as exec overwrites this code\n");
  
  // only get here if exec failed
  perror("ececv failed");

  // return error status  
  return 2;
}