#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 100

void main(void) {
  srand(time(NULL));
  int a[N];

  // entrance if 1
  for (int i = 0; i < N; i++) {
    a[i] = (rand() % 10 > 3) ? 1 : 0;
  }

  int tid;
  int count = 0;

  #pragma omp parallel num_threads(2) 
  {

    tid = omp_get_thread_num();

    if (tid == 0) {

      for (int j = 0; j < N; j++) {
        #pragma omp critical
        if (a[j] == 1) count++;
      }

    } else if (tid == 1) {

      for (int j = 0; j < N; j++) {
        #pragma omp critical
        if (a[j] == 0) count--;
      }    

    }

  }

  printf("passenger count left in the plane: %d\n", count);
  
}
