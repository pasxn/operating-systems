/*
 *
 * Purpose:  Use tree-structured communication to find the global sum of a random collection of ints.  
 *           This code only works for 2^n number of processors 
 * Compile:  mpicc ...
 * Run:      mpiexec ...
 *
 * Input:    None
 * Output:   Random values generated by processes, and their global sum.
 */


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm); 

const int MAX_CONTRIB = 20;

int main(void) {
  int i, sum, my_int;
  int my_rank, comm_sz;
  MPI_Comm comm;
  int* all_ints = NULL;
   
  MPI_Init(NULL, NULL);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &my_rank);

  srandom(my_rank + 1);
  my_int = random() % MAX_CONTRIB;
   
  sum = Global_sum(my_int, my_rank, comm_sz, comm);
   
  if (my_rank == 0) {
    all_ints = malloc(comm_sz*sizeof(int));
    
    /* Gather from each process each my_int to send back to process 0 to store all summands in array all_ints*/
    MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("Ints being summed:\n   ");
    
    for (i = 0; i < comm_sz; i++)
      printf("%d ", all_ints[i]);
    printf("\n");
    printf("Sum = %d\n",sum);
    
    free(all_ints);
  } else {
    /* Gather from each process each my_int to send back to process 0 to store all summands in array all_ints*/
    MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }
   
  MPI_Finalize();
  return 0;
} /* main */

/*-------------------------------------------------------------------
 * Function:   Global_sum
 * Purpose:    Implement a global sum using tree-structured communication
 * Notes:       
 * 1.  The return value is only valid on process 0
 */
int Global_sum(
    int my_int    /* in */,
    int my_rank   /* in */,
    int comm_sz   /* in */,
    MPI_Comm comm /* in */) {
   
  int partner, recvtemp;
  int my_sum = my_int;
  unsigned bitmask = 1;
   
  while (bitmask < comm_sz) {
    partner = my_rank ^ bitmask;
      
    if (my_rank < partner) {
      if (partner < comm_sz) {
        /* Recv value from partner into recvtemp */
        MPI_Recv(&recvtemp, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        my_sum += recvtemp;
	      bitmask <<= 1;
      }
    } else {
      /* Send value in my_sum to partner process */
      MPI_Send(&my_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      
      break;
    }
  }  /* while */

  return my_sum;
} /* Global_sum */
