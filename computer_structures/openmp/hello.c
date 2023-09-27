// gcc -fopenmp hello.c

#include <stdio.h>
#include <omp.h>

void main(void) {
  
  // num_threads() macro is optional
  #pragma omp parallel num_threads(7)
  {
    printf("hello world\n");
  }

}
