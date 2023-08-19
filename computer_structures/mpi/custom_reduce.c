#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void c_sum(int* a, int len, int* global_sum_sp) {
  for(int i = 0; i < len; i++)
    *global_sum_sp += *(a + i);
}

int main(int argc, char *argv[]) {
  srandom(time(NULL));

  if (argc < 2) {
    perror("comand line arguments are not entered");
    return 1;
  }

  const int num_cpus = atoi(argv[1]);
  const int len      = atoi(argv[2]);

  int a[len];
  int local_sum = 0;
  int global_sum_mp = 0;
  int global_sum_sp = 0;

  for (int i = 0; i < len; ++i) {
    // random numbers between 0 and 9
    a[i] = rand() % 10;
  }  

  int my_rank, comm_size;
  
  MPI_Comm comm;
  comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &comm_size);
  MPI_Comm_rank(comm, &my_rank);

  int scatter_size = len/(num_cpus);

  if(my_rank != 0) {
    int start = (my_rank - 1)*scatter_size;
    int end = (my_rank == (num_cpus-1)) ? len : (my_rank)*scatter_size;
    for(int i = start; i < end; i++) {
      local_sum += a[i];
    }
  }

  MPI_Reduce(&local_sum, &global_sum_mp, 1, MPI_INT, MPI_SUM, 0, comm);

  c_sum(a, len, &global_sum_sp);

  if(my_rank == 0) {
    printf("global sum sp: %d\n", global_sum_sp);
    printf("global sum mp: %d\n", global_sum_mp);    
  }

  MPI_Finalize(); 

  return 0;
}
