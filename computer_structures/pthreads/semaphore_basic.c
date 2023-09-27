// gcc -lpthread -lrt semaphore_basic.c

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NT 5
sem_t mutex;

void* thread(void* arg) {
  // wait
  sem_wait(&mutex);
  printf("\nentered...\n");

  // critical section
  sleep(4);

  // signal
  printf("\n\tjust exiting...\n");  
  sem_post(&mutex);

  pthread_exit(NULL);
}

void main(void) {
  sem_init(&mutex, 0, 1);

  pthread_t threads[NT];

  for (int i = 0; i < NT; i++){
    pthread_create(&threads[i], NULL, thread, NULL);
  }

  for (int i = 0; i < NT; i++){
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&mutex);
}
