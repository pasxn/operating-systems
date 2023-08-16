/*
 *
 * Purpose:  Use butterfly-structured communication to find the global sum of a random collection of ints.  This version is correct when comm_sz is not a power of 2.
 *
 * Compile:  mpicc ...
 * Run:      mpiexec ...
 *
 * Input:    None
 * Output:   Random values generated by processes, and global sum computed by each process.
 */


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm); 
int Floor_log(int comm_sz);

const int MAX_CONTRIB = 20;

int main(void) {
   int i, sum, my_int;
   int my_rank, comm_sz;
   MPI_Comm comm;
   int* all_ints = NULL; 
   int* sum_proc = NULL;
   
   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   srandom(my_rank + 1);
   my_int = random() % MAX_CONTRIB;
   
   sum = Global_sum(my_int, my_rank, comm_sz, comm);
   
   if ( my_rank == 0) {
      all_ints = malloc(comm_sz*sizeof(int)); 
      sum_proc = malloc(comm_sz*sizeof(int));
      /* Gather from each process each my_int to send back to process 0 to store all summands in array all_ints */
      MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, MPI_COMM_WORLD);
      printf("Ints being summed:\n   ");
      for (i = 0; i < comm_sz; i++)
         printf("%d ", all_ints[i]);
      printf("\n");
      /* Gather from each process each sum to send back to process 0 and store each processes' sum in array sum_proc */
      MPI_Gather(&sum, 1, MPI_INT, sum_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
      printf("Sums on the processes:\n   ");
      for (i = 0; i < comm_sz; i++)
         printf("%d ", sum_proc[i]);
      printf("\n");
      free(all_ints);
   } else {
   	MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, MPI_COMM_WORLD);
   	MPI_Gather(&sum, 1, MPI_INT, sum_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
      /* Gather from each process each my_int to send back to process 0 and store all summands in array all_ints */
      /* Gather from each process each sum to send back to process 0 and store each processes' sum in array sum_proc */
   }
   
   MPI_Finalize();
   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:   Global_sum
 * Purpose:    Implement a global sum using butterfly-structured communication
 * Notes:       
 * 1.  The return value is valid on all the processes
 */
int Global_sum(
      int my_int    /* in */,
      int my_rank   /* in */,
      int comm_sz   /* in */,
      MPI_Comm comm /* in */) {
   
   int partner, recvtemp;
   int my_sum = my_int;
   unsigned bitmask = 1;
   int floor_log_p = Floor_log(comm_sz);
   MPI_Status status;

   /* Get values from processes with rank >= floor_log_p */
   if (my_rank >= floor_log_p) {
      partner = my_rank - floor_log_p;
      MPI_Send(&my_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
      /* Send value in my_sum to partner process */
   } else {
      partner = my_rank + floor_log_p;
      if (partner < comm_sz) {
      	MPI_Recv(&recvtemp, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);
         /* Recv value from partner into recvtemp */ 
         my_sum += recvtemp;
      }
   }
   
   /* Butterfly with processes with rank < floor_log_p */
   if (my_rank < floor_log_p)
      while (bitmask < floor_log_p) {
         partner = my_rank ^ bitmask;
         MPI_Send(&my_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
         MPI_Recv(&recvtemp, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);
         /* Send value from my_sum to partner process and receive value from partner into recvtemp */ 
         my_sum += recvtemp;
         bitmask <<= 1;
      }  /* while */

   /* Send result to processes with rank >= floor_log_p */
   if (my_rank >= floor_log_p) {
      partner = my_rank - floor_log_p;
      MPI_Recv(&my_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);
      /* Recv value from partner into my_sum */
   } else {
      partner = my_rank + floor_log_p;
      if (partner < comm_sz)
          MPI_Send(&my_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
         /* Send value in my_sum to partner process */
   }

   return my_sum;
}  /* Global_sum */


//-------------------------------------------------------------------/
int Floor_log(int comm_sz) {
   int fl = 1;

   while (2*fl <= comm_sz)
      fl *= 2;

   return fl;
}  /* Floor_log */