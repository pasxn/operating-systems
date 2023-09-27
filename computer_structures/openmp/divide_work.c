#include <stdio.h>
#include <omp.h>

void main(void) {
  
  #pragma omp parallel sections num_threads(3)
  { 
    #pragma omp section
    printf("hello world one\n");

    #pragma omp section
    printf("hello world two\n");

    #pragma omp section
    printf("hello world three\n");        
  }

}
