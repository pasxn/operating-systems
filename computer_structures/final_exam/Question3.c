// EN20421380

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int process_count;
double a, b, h;
int n, local_n;
double total;

// ---------- function parametere ----------
// to calculate the local integral
double Trap(double local_a, double local_b, int local_n, double h);
// integrating function
double f(double x);

void main(int argc, char** argv) {
  int my_rank, comm_sz;
 
  MPI_Init(NULL, NULL);
  MPI_Comm comm;
 
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &my_rank);
  
  // getting input only in the process 1
  if (my_rank == 0) {
    printf("my_rank = %d\n", my_rank);
    printf("Enter a, b, n \n");
    scanf("%lf %lf %d", &a, &b, &n);
    
    // braodcasting values to every process
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    // braodcasting values to every process
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);   
  }
  
  h = (b - a)/n;
  local_n = n/comm_sz;
  
  // left endpoint for each process
  double local_a = a + my_rank * local_n * h;
  
  // right endpoint for each process
  double local_b = local_a + local_n * h;
  
  // integral over local interval
  double my_int = Trap(local_a, local_b, local_n, h);
  
  MPI_Reduce(&my_int, &total, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
  
  if (my_rank == 0) {
    printf("integral from %lf to %lf = %lf\n", a, b, total);
  }
  
  MPI_Finalize();
}

double Trap(double local_a, double local_b, int local_n, double h) {

  // store result in this variable
  double integral;
  double x;
  int i;
  
  integral = (f(local_a) + f(local_b))/2.0;
  x = local_a;
  
  for (i = 1; i <= local_n - 1; i++) {
    x = local_a + i * h;
    integral += f(x);
  }
  
  integral =  integral * h;
  
  return integral;
}

double f(double x) {
  return x*x + 5*x + 10;
}
