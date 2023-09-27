#include <stdio.h>
#include <omp.h>

void main(void) {
  
  int x = 0;
  
  #pragma omp parallel num_threads(10)
  {
    #pragma omp critical
    {
      x++;
      printf("current val : %d\n", x);
    }
  }

  printf("--------------------------------\n");
  printf("final val: %d\n", x);

}
