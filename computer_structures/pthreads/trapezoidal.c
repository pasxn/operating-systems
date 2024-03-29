/* 
 * File:     lab4
 * Purpose:  Compute a definite integral using the trapezoidal rule and Pthreads.
 *
 * Input:    a, b, n
 * Output:   Estimate of the integral from a to b of f(x) using the trapezoidal rule and n trapezoids.
 *
 * Compile:  gcc ...
 * Usage:    ./lab4 <number of threads> <method>
 *
 * Algorithm:
 *    1.  Each thread calculates "its" interval of integration.
 *    2.  Each thread estimates the integral of f(x) over its interval using the trapezoidal rule.
 *    3.  Each thread adds in its calculation into the global total
 *    4.  main prints result
 *
 * Note:  
 *    1.  f(x) is hardwired
 *    2.  Assumes the number of threads evenly divides the number of trapezoids.
 *    3.  Method 1 (default): uses mutex
 *	      Method 2: uses semaphore
 *	      Method 3: uses busy-wait
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* The global variables are shared among all the threads. */
int thread_count;
int n, local_n;
int method;
double a, b, h;

/* Critical section lock variables */
int flag;
sem_t sem;
pthread_mutex_t mutex;
double total;

void* thread_work(void* rank);

/* Calculate local integral  */
double trap(double local_a, double local_b, int local_n, double h);

/* function we're integrating */
double f(double x);

/*--------------------------------------------------------------*/
int main(int argc, char** argv) {
  long i;
  pthread_t*  thread_handles;

  total = 0.0;
  if (argc != 3) {
    fprintf(stderr, "usage: %s <number of threads> <method>\n", argv[0]);
    exit(0);
  }
  thread_count = strtol(argv[1], NULL, 10);
  method = strtol(argv[2], NULL, 10);

  printf("Enter a, b, n\n");
  scanf("%lf %lf %d", &a, &b, &n);
  h = (b - a)/n;
  local_n = n/thread_count;

  /* Allocate storage for thread handles. */
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  /* Initialize the mutex, semaphore, busy-wait */
  flag = 0;
  pthread_mutex_init(&mutex, NULL);
  sem_init(&sem, 0, 1);

  /* Start the threads. */
  for (i = 0; i < thread_count; i++) {
    /* create thread with attribute thread_handle[i], executing the function thread_work, with rank i */
    pthread_create(&thread_handles[i], NULL, thread_work, &i);
  }

  /* Wait for threads to complete. */
  for (i = 0; i < thread_count; i++) {
    /* join corresponding threads */
    pthread_join(thread_handles[i], NULL);
  }

  printf("With n = %d trapezoids, our estimate\n", n);
  printf("of the integral from %f to %f = %19.15e\n", a, b, total);

  pthread_mutex_destroy(&mutex);
	sem_destroy(&sem);
  free(thread_handles);

  return 0;
} /*  main  */

/*--------------------------------------------------------------*/
void* thread_work(void* rank) {
  double local_a;   /* Left endpoint my thread   */
  double local_b;   /* Right endpoint my thread  */
  double  my_int;   /* Integral over my interval */
  long my_rank = *(long*)(rank);

  /* Length of each process' interval of integration = local_n*h.  So my interval starts at: */
  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;

  my_int = trap(local_a, local_b, local_n, h);

  switch (method) {
    case 2:
	    /* semaphore critical section to add local integral to total */
      sem_wait(&sem);
      total += my_int;
      sem_post(&sem);
	    break;
	  case 3:
	    /* busy-wait critical section to add local integral to total */
      while (__sync_lock_test_and_set(&flag, 1) == 1);
      total += my_int;
      __sync_lock_release(&flag);
	    break;
	  default:
	    /* mutex critical section to add local integral to total */
      pthread_mutex_lock(&mutex);
      total += my_int;
      pthread_mutex_unlock(&mutex);
	    break;
    }
    
    return NULL;
	
}  /* thread_work */

/*--------------------------------------------------------------*/
double trap(
			double  local_a   /* in */,
			double  local_b   /* in */,
			int     local_n   /* in */,
			double  h         /* in */) {
	
  double integral;   /* Store result in integral  */
  double x;
  int i;

  integral = (f(local_a) + f(local_b))/2.0;
  x = local_a;

  for (i = 1; i <= local_n - 1; i++) {
    x = local_a + i * h;
    integral += f(x);
  }

  integral = integral*h;
  return integral;
} /*  trap  */


/*--------------------------------------------------------------*/
double f(double x) {
  return x*x;
} /* f */
