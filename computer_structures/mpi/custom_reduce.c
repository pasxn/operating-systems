#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void main(int argc, char *argv[]) {
  srandom(time(NULL));

  int my_rank, comm_size;
  
  MPI_Comm comm;
  comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &comm_size);
  MPI_Comm_rank(comm, &my_rank);

  

  MPI_Finalize();
}