#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 5000000

long long sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* counting_function(void* arg) {
  int offset = *(int*)arg;

  for (int i = 0; i < ITERATIONS; i++) {
    pthread_mutex_lock(&mutex);
    sum =+ offset;
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

void main(void) {
  pthread_t threads[2];
  
  int offset_one =  1;
  int offset_two = -1;

  pthread_create(&threads[0], NULL, counting_function, &offset_one);
  pthread_create(&threads[1], NULL, counting_function, &offset_two);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  printf("sum: %lld\n", sum);
}
