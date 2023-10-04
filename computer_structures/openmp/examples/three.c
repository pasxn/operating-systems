#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* The global variables are shared among all the threads. */
int thread_count;
int n, local_n;
double a, b, h;

double total;

void thread_work();

/* Calculate local integral  */
double trap(double local_a, double local_b, int local_n, double h);

/* function we're integrating */
double f(double x);

/*--------------------------------------------------------------*/
int main(int argc, char** argv) {
  long i;
  pthread_t*  thread_handles;

  total = 0.0;
  if (argc != 2) {
    fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
    exit(0);
  }
  thread_count = strtol(argv[1], NULL, 10);

  a = 0; b = 5; n = 10000;

  h = (b - a)/n;
  local_n = n/thread_count;

  /* Allocate storage for thread handles. */
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  #pragma omp parallel num_threads(thread_count)
  thread_work();

  printf("With n = %d trapezoids, our estimate\n", n);
  printf("of the integral from %f to %f = %f\n", a, b, total);

  free(thread_handles);

  return 0;
} /*  main  */

/*--------------------------------------------------------------*/
void thread_work() {
  double local_a;   /* Left endpoint my thread   */
  double local_b;   /* Right endpoint my thread  */
  double  my_int;   /* Integral over my interval */
  int tid;

  tid = omp_get_thread_num();
  long my_rank = tid;

  /* Length of each process' interval of integration = local_n*h.  So my interval starts at: */
  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;

  my_int = trap(local_a, local_b, local_n, h);

  /* critical section is handled by this */
  #pragma omp critical 
  total += my_int;
    
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
  return (x*x) + 1;
} /* f */
