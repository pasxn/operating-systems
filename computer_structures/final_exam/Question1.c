// EN20421380

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

// define the number of threads a 4
#define NUM_THREADS 4

#define ROWS 4
#define COLS 100

int array[ROWS][COLS];

// create an array to store the max value of the each row by independent threads
int max_each_row[ROWS];

// create a variable to store the global max
int max_all = 0;

void max_local(int row) {
  int max = 0;
  
  for (int i = 0; i < COLS; ++i) {
    // check the local max and store in the location of the corresponding thread id in max_each_row array
    max = array[row][i] > max ? array[row][i] : max;
    
    // check the global max and store in max_all variable
    
    // since max_all variable is accessed by many threads at once it's a critical section
    // therefore use #pragma omp critical to implement mutual exclusion to overcome a race condition
    #pragma omp critical
    max_all = array[row][i] > max_all ? array[row][i] : max_all;
  }
  
  max_each_row[row] = max;
  
  printf("maximun value in the row %d is %3d\n", row, max);
}


void main(int argc, char** argv) {
  
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      array[i][j] = rand() % 1000;
    }
  }
  
  // parallelize the max_local() function in 4 threads
  #pragma omp parallel num_threads(NUM_THREADS)
  max_local(omp_get_thread_num());
  
  printf("overall maximum value of the 2d array: %d\n", max_all);
}
