#include <stdio.h>
#include <omp.h>

#define M 5
#define N 10

int array[M][N];
int sums[N] = {0};

int global_sum = 0;

void print_array() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d\t", array[i][j]);
    }
    puts("\n");
  }
}

void thread_func(int col) {
  for (int i = 0; i < M; i++) {
    sums[col] += array[i][col];
  }

  printf("sum at thread %3d is %3d\n", col, sums[col]);
}

void main(void) {

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      array[j][i] = (i+1)*10;
    }
  }

  print_array();

  #pragma omp parallel num_threads(N)
  {
    int tid = omp_get_thread_num();
    thread_func(tid);
  }

  for (int i = 0; i < N; i++) {
    global_sum += sums[i];
  }

  printf("global sum: %d\n", global_sum);

}
