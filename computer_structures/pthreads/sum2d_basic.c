#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define M 5
#define N 100

int mat[M][N];
int total = 0;
pthread_mutex_t mutex;

void* sum(void* arg) {
  for (int i = 0; i < N; i++) {
    pthread_mutex_lock(&mutex);
    total += mat[*((int*)arg)][i];
    pthread_mutex_unlock(&mutex);
  }
  printf("%d\n", *((int*)arg));
  pthread_exit(NULL);
}

void main(void) {
  srandom(time(NULL));

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      mat[i][j] = rand() % 5;
    }
  }

  pthread_t threads[M];
  int data[M] = {0, 1, 2, 3, 4};

  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < M; i++) {
    pthread_create(&threads[i], NULL, sum, &data[i]);
  }

  for (int i = 0; i < M; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("total : %d\n", total);

  pthread_mutex_destroy(&mutex);
}
