#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Global_sum(int num, int my_rank, int comm_sz, MPI_Comm comm); 

int main(void) {
   int i, sum, num;
   int my_rank, comm_sz;
   MPI_Comm comm;
   int* all_ints = NULL; 
   int* sum_proc = NULL;
   
   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   num = my_rank;
   
   sum = Global_sum(num, my_rank, comm_sz, comm);
   
   if (my_rank == 0) {
      all_ints = malloc(sizeof(int) * comm_sz); 
      sum_proc = malloc(sizeof(int) * comm_sz);
      
      /* Gather from each process each num to send back to process 0 to store all summands in array all_ints */
      MPI_Gather(&num, 1, MPI_INT, all_ints, 1, MPI_INT, 0, MPI_COMM_WORLD);
      
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
      free(sum_proc);
   } else {
      /* Gather from each process each num to send back to process 0 and store all summands in array all_ints */
      /* Gather from each process each sum to send back to process 0 and store each processes' sum in array sum_proc */
      MPI_Gather(&num, 1, MPI_INT, all_ints, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Gather(&sum, 1, MPI_INT, sum_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);   
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
      int num    /* in */,
      int my_rank   /* in */,
      int comm_sz   /* in */,
      MPI_Comm comm /* in */) {
   
   int partner, recvtemp;
   int my_sum = num;
   unsigned int bitmask = 1;
   MPI_Status status;
   
   /* Butterfly with processes with rank < floor_log_p */
   while (bitmask < comm_sz) {
     partner = my_rank ^ bitmask;

     /* Send value from my_sum to partner process and receive value from partner into recvtemp */
     MPI_Sendrecv(&my_sum, 1, MPI_INT, partner, 0, &recvtemp, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);

     my_sum += recvtemp;
     bitmask <<= 1;
   }  /* while */

   return my_sum;
}  /* Global_sum */
