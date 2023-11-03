// EN20421380

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 3
#define TCOUNT 10
#define COUNT_LIMIT 30

int count = 0;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void* inc_count(void* t) {
  int i;
  long my_id = (long)t;
  
  for (i = 0; i < TCOUNT; i++) {
    pthread_mutex_lock(&count_mutex);
    
    /* Check the thread id and add the corresponding value according to the question in each thread */
    if (my_id == 2) count += 5;
    if (my_id == 3) count += 10;
    
    /*
    Check the value of count and signal waiting thread when condition is reached.
    This occurs while mutex is locked! 
    */
    if (count == COUNT_LIMIT) {
      printf("inc_count(): thread %ld, count = %d Threashold reached. ", my_id, count);
      pthread_cond_signal(&count_threshold_cv);
      printf("Just sent signal.\n");
    }
    
    printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
    pthread_mutex_unlock(&count_mutex);
    
    /* Do some work so the threads can alternate on mutex lock*/
    sleep(1);
  }
  
  pthread_exit(NULL);
}

void* watch_count(void* t) {
  long my_id = (long)t;
  printf("starting watch count(): thread %ld\n", my_id);
  
  /*
  Lock mutex wait for the signal.
  pthread_cond_wait automatically and atomically unlock mutex while it waits!
  Also, if COUNT_LIMIT is reached before this routine is run by the following thread,
  the loop will be skipped to prevent the pthread_cond_wait from never returning.
  */
  pthread_mutex_lock(&count_mutex);
  
  while (count < COUNT_LIMIT) {
    printf("watch_count(): thread %ld count = %d. going into wait...\n", my_id, count);
    pthread_cond_wait(&count_threshold_cv, &count_mutex);
    printf("watch_count(): thread %ld condition signal received. count = %d\n", my_id, count);
  }
  
  printf("watch_count(): thread %ld updating the value of count...\n", my_id);
  
  count += 50;
  
  printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
  printf("watch_count(): thread %ld,unlocking mutex\n", my_id);
  
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);
}

void main(int argc, char** argv) {
  int i, rc;
  long t1 = 1, t2 = 2, t3 = 3;
  pthread_t threads[3];
  
  /* initialize the mutex and condition variable instances */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init(&count_threshold_cv, NULL);
  
  /* Explicity create threads */
  pthread_create(&threads[0], NULL, watch_count, (void*)t1);
  pthread_create(&threads[1], NULL, inc_count,   (void*)t2);
  pthread_create(&threads[0], NULL, inc_count,   (void*)t3);
  
  /* Wait for the threads to complete */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  printf("Main: waited and joined with %d threads. final value of count = %d. Done.\n", NUM_THREADS, count);
  
  /* Clean up and exit */
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  pthread_exit(NULL);
}
