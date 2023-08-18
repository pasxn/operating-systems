#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


// define a structure to hold thread-specific data
typedef struct _thread_data {
  int* a;
  int* b;
  int* c;
  int rows_a;
  int cols_a;
  int cols_b;
  int start_row;
  int end_row;
} thread_data;

// thread function for matrix multiplication
void* thread_gemm(void* arg) {
  thread_data* data = (thread_data*)arg;

  for (int i = data->start_row; i < data->end_row; i++) {
    for (int j = 0; j < data->cols_b; j++) {
      *(data->c + i * data->cols_b + j) = 0;
      for (int k = 0; k < data->cols_a; k++) {
        *(data->c + i * data->cols_b + j) +=
          (*(data->a + i * data->cols_a + k)) * (*(data->b + k * data->cols_b + j));
      }
    }
  }

  pthread_exit(NULL);
}

void mt_gemm(int* a, int* b, int* c, int rows_a, int cols_a, int cols_b, int num_threads) {
  pthread_t threads[num_threads];
  thread_data thread_data[num_threads];
  int rows_per_thread = rows_a / num_threads;

  for (int t = 0; t < num_threads; t++) {
    thread_data[t].a = a;
    thread_data[t].b = b;
    thread_data[t].c = c;
    thread_data[t].rows_a = rows_a;
    thread_data[t].cols_a = cols_a;
    thread_data[t].cols_b = cols_b;
    thread_data[t].start_row = t * rows_per_thread;
    thread_data[t].end_row = (t == num_threads - 1) ? rows_a : (t + 1) * rows_per_thread;

    pthread_create(&threads[t], NULL, thread_gemm, &thread_data[t]);
  }

  for (int t = 0; t < num_threads; t++) {
    pthread_join(threads[t], NULL);
  }
}

void st_gemm(int* a, int* b, int* c, int rows_a, int cols_a, int cols_b) {
  for (int i = 0; i < rows_a; i++) {
    for (int j = 0; j < cols_b; j++) {
      *(c + i * cols_b + j) = 0;
      for (int k = 0; k < cols_a; k++) {
        *(c + i * cols_b + j) += (*(a + i * cols_a + k)) * (*(b + k * cols_b + j));
      }
    }
  }
}

void print_matrix(int* matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%6d ", *(matrix + i * cols + j));
    }
    printf("\n");
  }
}

void main(int argc, char *argv[]) {
  srand(time(NULL));

  char *debugValue = getenv("DEBUG");

  struct timespec start_time_st, end_time_st, start_time_mt, end_time_mt;
  long elapsed_nanoseconds_st, elapsed_nanoseconds_mt;

  int num_cpu = sysconf(_SC_NPROCESSORS_ONLN);

  const int rows = atoi(argv[1]);
  const int cols = atoi(argv[2]);
  
  int a[rows][cols];
  int b[cols][rows];
  int c[rows][rows];

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      // random numbers between 0 and 99
      a[i][j] = rand() % 100;
    }
  }

  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      // random numbers between 0 and 99
      b[i][j] = rand() % 100;
    }
  }

  // single threaded time
  clock_gettime(CLOCK_MONOTONIC, &start_time_st);
  st_gemm((int*)a, (int*)b, (int*)c, rows, cols, rows);
  clock_gettime(CLOCK_MONOTONIC, &end_time_st);

  elapsed_nanoseconds_st = (end_time_st.tv_sec - start_time_st.tv_sec) * 1000000000 + \
                           (end_time_st.tv_nsec - start_time_st.tv_nsec);

  // multi threaded time
  clock_gettime(CLOCK_MONOTONIC, &start_time_mt);
  mt_gemm((int*)a, (int*)b, (int*)c, rows, cols, rows, num_cpu);
  clock_gettime(CLOCK_MONOTONIC, &end_time_mt);

  elapsed_nanoseconds_mt = (end_time_mt.tv_sec - start_time_mt.tv_sec) * 1000000000 + \
                           (end_time_mt.tv_nsec - start_time_mt.tv_nsec);

  if (debugValue != NULL && strcmp(debugValue, "1") == 0) {
    printf("matrix a:\n");
    print_matrix((int*)a, rows, cols);

    printf("\nmatrix b:\n");
    print_matrix((int*)b, cols, rows);

    printf("\nmatrix c (Result of a * b):\n");
    print_matrix((int*)c, rows, rows);
    printf("\n\n");
  }

  printf("elapsed eime (single threaded): %ld ms\n", elapsed_nanoseconds_st/1000000);
  printf("elapsed eime (multi threaded) : %ld ms\n", elapsed_nanoseconds_mt/1000000);
}
