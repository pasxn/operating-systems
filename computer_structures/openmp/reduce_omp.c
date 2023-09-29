#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <omp.h>

void floor_log(int size, int* log) {
   *log = 1;

   while (2*(*log) <= size)
      (*log) *= 2;
}

void reduce_mt(int* a, int a_len_, int* c, const int nt) {
  int a_len;
  floor_log(a_len_, &a_len);

  *c = 0;

  omp_lock_t writelock;

  omp_init_lock(&writelock);

  for (int i = 0; i < a_len; i += nt) {
    #pragma omp parallel num_threads(nt)
    {
      int tid = omp_get_thread_num();

      omp_set_lock(&writelock);
      *c += *(a + tid + i);
      omp_unset_lock(&writelock);
    }
  }
  
  for (int i = a_len; i < a_len_; i++) {
    *c += *(a + i);
  }

  omp_destroy_lock(&writelock);

}

void reduce_st(int* a, int a_len, int* c) {
  *c = 0;
  for(int i = 0; i < a_len; i++)
    *c += *(a + i);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  struct timespec start_time_st, end_time_st, start_time_mt, end_time_mt;
  long elapsed_nanoseconds_st, elapsed_nanoseconds_mt;  

  int num_cpu = sysconf(_SC_NPROCESSORS_ONLN);

  if (argc < 2) {
    perror("comand line arguments are not entered");
    return 1;
  }

  const int len = atoi(argv[1]);
  int a[len];

  for (int i = 0; i < len; ++i) {
    // random numbers between 0 and 9
    a[i] = rand() % 10;
  }

  int result_st, result_mt;

  // single threaded time
  clock_gettime(CLOCK_MONOTONIC, &start_time_st);
  reduce_st(a, len, &result_st);
  clock_gettime(CLOCK_MONOTONIC, &end_time_st);

  elapsed_nanoseconds_st = (end_time_st.tv_sec - start_time_st.tv_sec) * 1000000000 + \
                           (end_time_st.tv_nsec - start_time_st.tv_nsec);

  // multi threaded time
  clock_gettime(CLOCK_MONOTONIC, &start_time_mt);    
  reduce_mt(a, len, &result_mt, num_cpu);
  clock_gettime(CLOCK_MONOTONIC, &end_time_mt);

  elapsed_nanoseconds_mt = (end_time_mt.tv_sec - start_time_mt.tv_sec) * 1000000000 + \
                           (end_time_mt.tv_nsec - start_time_mt.tv_nsec);

  printf("single threaded output: %d\n", result_st);
  printf("multi threaded output : %d\n", result_mt);

  printf("-----------------------------------------------------------\n");

  printf("elapsed eime (single threaded): %ld us\n", elapsed_nanoseconds_st/1000);
  printf("elapsed eime (multi threaded) : %ld us\n", elapsed_nanoseconds_mt/1000);  

}
