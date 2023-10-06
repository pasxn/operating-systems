#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 8


long long int Parallelize(long long int local_number_of_tosses);
long long int Monte_carlo(long long number_of_tosses, int tid);

int main(void) {
  long long int number_of_tosses;
  long long int local_number_of_tosses;
  long long int number_in_circle;
  long long int local_number_in_circle;

  double pi_estimate;

  printf("Enter the total number of tosses\n");
  scanf("%lld", &number_of_tosses);

  local_number_of_tosses = number_of_tosses/NUM_THREADS;

  number_in_circle = Parallelize(local_number_of_tosses);
  
  pi_estimate = 4 * number_in_circle/((double)number_of_tosses);
  printf("pi estimate = %f\n", pi_estimate);

  return 0;
}

long long int Monte_carlo(long long local_number_of_tosses, int tid) {
  long long int i;
  double x,y;
  double distance_squared;
  long long int local_number_in_circle = 0;

  srandom(tid + 1);

  for ( i=0 ; i< local_number_of_tosses ; i++) {
    x = (double)random()/RAND_MAX; /* x= random between -1 and 1 */
    y = (double)random()/RAND_MAX; /* y= random between -1 and 1 */

    distance_squared = (x*x) + (y*y) ;
    
    /* distance_squared = distance squared of dart toss from centre position */
    if(distance_squared <= 1){
      /* if dart falls in unit circle, increment the count local_number_in_circle */
      local_number_in_circle += 1;
    }
  }

  return local_number_in_circle;
}

long long int Parallelize(long long int local_number_of_tosses) {
  
  long long int number_in_circle = 0;

  #pragma omp parallel num_threads(NUM_THREADS)
  {
    #pragma omp critical
    number_in_circle += Monte_carlo(local_number_of_tosses, omp_get_thread_num());
  }

  return number_in_circle;
}
