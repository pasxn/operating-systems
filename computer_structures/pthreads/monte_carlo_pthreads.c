// gcc -lpthread monte_carlo.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 8

long long int number_of_tosses = 0;
long long int local_number_of_tosses = 0;
long long int local_number_in_circle = 0;
long long int number_in_circle = 0;

pthread_mutex_t mutex;

void Parallelize(long long int local_number_of_tosses);
void* Monte_carlo(void* arg);

int main(void) {

  double pi_estimate;

  printf("Enter the total number of tosses: ");
  scanf("%lld", &number_of_tosses);

  local_number_of_tosses = number_of_tosses/NUM_THREADS;

  Parallelize(local_number_of_tosses);
  
  pi_estimate = 4 * number_in_circle/((double)number_of_tosses);
  printf("pi estimate: %f\n", pi_estimate);

  return 0;
}

void* Monte_carlo(void* arg) {
  int tid = *((int*)arg);
  long long int i;
  double x, y;
  double distance_squared;
  long long int local_number_in_circle = 0;

  srandom(tid + 1);

  for ( i=0 ; i < local_number_of_tosses ; i++) {
    x = (double)random()/RAND_MAX; /* x= random between -1 and 1 */
    y = (double)random()/RAND_MAX; /* y= random between -1 and 1 */

    distance_squared = (x*x) + (y*y);
    
    /* distance_squared = distance squared of dart toss from centre position */
    if(distance_squared <= 1) {
      /* if dart falls in unit circle, increment the count local_number_in_circle */
      local_number_in_circle += 1;
    }
  }

  printf("number of tosses: %lld process id: %d\n", local_number_in_circle, tid);

  pthread_mutex_lock(&mutex);
  number_in_circle += local_number_in_circle;
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

void Parallelize(long long int local_number_of_tosses) {
  pthread_mutex_init(&mutex, NULL);

  pthread_t threads[NUM_THREADS];
  int thread_data[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++){
    thread_data[i] = i;
    pthread_create(&threads[i], NULL, Monte_carlo, &thread_data[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
}
