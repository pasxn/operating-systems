#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Get_input(long long int* number_of_tosses, int my_rank, MPI_Comm comm);
long long int Monte_carlo(long long number_of_tosses, int my_rank);

int main(void) {
  long long int number_of_tosses;
  long long int local_number_of_tosses;
  long long int number_in_circle;
  long long int local_number_in_circle;

  double pi_estimate;

  int my_rank, comm_sz;
  MPI_Comm comm;

  MPI_Init(NULL, NULL);
  comm = MPI_COMM_WORLD;

  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &my_rank);

  Get_input(&number_of_tosses, my_rank, comm);

  local_number_of_tosses = number_of_tosses/comm_sz;
  local_number_in_circle = Monte_carlo(local_number_of_tosses, my_rank);

  MPI_Reduce(&local_number_in_circle, &number_in_circle, 1, MPI_LONG_LONG,MPI_SUM, 0, MPI_COMM_WORLD);
  
  if (my_rank == 0) {
    pi_estimate = 4 * number_in_circle/((double)number_of_tosses);
    printf("pi estimate = %f\n", pi_estimate);
  }

  MPI_Finalize();

  return 0;
}

void Get_input(long long int* number_of_tosses, int my_rank, MPI_Comm comm) {
  if (my_rank == 0) {
    printf("Enter the total number of tosses\n");
    scanf("%lld", number_of_tosses);
  }

  MPI_Bcast(number_of_tosses,1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
}

long long int Monte_carlo(long long local_number_of_tosses, int my_rank) {
  long long int i;
  double x,y;
  double distance_squared;
  long long int number_in_circle = 0;

  srandom(my_rank+1);

  for ( i=0 ; i< local_number_of_tosses ; i++) {
    x = (double)random()/RAND_MAX; /* x= random between -1 and 1 */
    y = (double)random()/RAND_MAX; /* y= random between -1 and 1 */

    distance_squared = (x*x) + (y*y) ;
    
    /* distance_squared = distance squared of dart toss from centre position */
    if(distance_squared <= 1){
      /* if dart falls in unit circle, increment the count number_in_circle */
      number_in_circle += 1;
    }
  }

  return number_in_circle;
}
