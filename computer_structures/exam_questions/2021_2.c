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

  my_int = my_rank;
  sum = Global_sum(my_int, my_rank, comm_sz, comm);

  if (my_rank == 6) {
    all_ints = malloc(comm_sz * sizeof(int));
    
    MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 6, comm);
    printf("Ints being summed by p3:\n ");
    
    for (i = 0; i < comm_sz; i++)
      printf("%d ", all_ints[i]);
    printf("\n");
    
    printf("Sum = %d\n", sum);
  } else {
    MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 6, comm);
  }

  MPI_Finalize();
  return 0;
}

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm) {
  int partner, recvtemp;
  int my_sum = my_int;
  unsigned bitmask = 1;

  while (bitmask < comm_sz) {
    partner = my_rank ^ bitmask;
    if (my_rank > partner) {
      if (partner < comm_sz) {
        MPI_Recv(&recvtemp, 1, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
        my_sum += recvtemp;
      }
      bitmask <<= 1;
    } else {
      MPI_Send(&my_sum, 1, MPI_INT, partner, 0, comm);
      break;
    }
  }
  return my_sum;
}
