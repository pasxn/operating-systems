#include <stdio.h>
#include <omp.h>

#define N 10

void main(void) {
  int a[N];
  int b[N];
  int c[N];

  for (int i = 0; i < N; i++) {
    a[i] = 1;
    b[i] = 1;
  }

  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
  }  

  for (int i = 0; i < N; i++) {
    printf("%d\t", c[i]);
  }
  printf("\n");

}