#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


void encrypt(int* a, int* b) {
  *b = *a ^ 8;
}

void main(int argc, char *argv[]) {
  srandom(time(NULL));

  int my_rank, comm_size;
  
  MPI_Comm comm;
  comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &comm_size);
  MPI_Comm_rank(comm, &my_rank);

  if (my_rank == 0) {
    int my_int = random() % 20;
    int received_encrypted_result = 0;
    MPI_Send(&my_int, 1, MPI_INT, 1, 0, comm);
    MPI_Recv(&received_encrypted_result, 1, MPI_INT, 1, 1, comm, MPI_STATUS_IGNORE);

    printf("send number     : %d\n", my_int);
    printf("received number : %d\n", received_encrypted_result);

  } else {
    int result = 0;
    int encrypted_result = 0;

    MPI_Recv(&result, 1, MPI_INT, 0, 0, comm, MPI_STATUS_IGNORE);
    encrypt(&result, &encrypted_result);
    MPI_Send(&encrypted_result, 1, MPI_INT, 0, 1, comm);
  }

  MPI_Finalize();
}