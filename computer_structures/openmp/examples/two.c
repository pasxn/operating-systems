#include <stdio.h>
#include <omp.h>

#define N 10

void f(int x, int* y) {
  *y = (x * x) + (x * 2) + 3;
}

void main(void) {
  
  int tid;
  int y_mt[N] = {0};
  int y_st_temp;
  
  #pragma omp parallel num_threads(N)
  {
    tid = omp_get_thread_num();

    f(tid + 1, (y_mt + tid));
  }

  for (int i = 0; i < N; i++) {
    f(i + 1, &y_st_temp);
    
    if (y_st_temp != y_mt[i]) {
      printf("assertion error at the element %d!\n", i);
    }

    printf("result single threaded: %3d | result multi threaded : %3d\n", y_st_temp, y_mt[i]);
  }

}
