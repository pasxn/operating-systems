#include <stdio.h>
#include <omp.h>

void main(void) {
  
  int a[5] = {1, 2, 3, 4, 5};
  int b[5] = {1, 2, 3, 4, 5};
  int c[5];

  int tid;

  #pragma omp parallel num_threads(5)
  {
    tid = omp_get_thread_num();

    c[tid] = a[tid] + b[tid];

    printf("c[%d] : %d\n", tid, c[tid]);
  }
}
