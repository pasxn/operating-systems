#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


// define a structure to hold thread-specific data
typedef struct _thread_data {
  int* a;
  int* answer;
  int start;
  int end;
} thread_data;

void* reduce_thread(void* arg) {
  thread_data* data = (thread_data*)arg;

  for(int i = data->start; i < data->end; i++) {
    *data->answer += data->a[i];
  }

  pthread_exit(NULL);
}

void reduce_mt(int* a, int a_len, int* c, const int num_threads) {
  *c = 0;
  pthread_t thread_id[num_threads];
  thread_data td[num_threads];
  int range = a_len/num_threads;

  for(int i = 0; i < num_threads; i++) {
    td[i].a = a;
    td[i].answer = c;
    td[i].start = range * i;
    td[i].end = (i == num_threads - 1) ? a_len : (i + 1) * range;

    pthread_create(&thread_id[i], NULL, reduce_thread, &td[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(thread_id[i], NULL);
  }
}

void reduce_st(int* a, int a_len, int* c) {
  *c = 0;
  for(int i = 0; i < a_len; i++)
    *c += *(a + i);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

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

  reduce_st(a, len, &result_st);
  reduce_mt(a, len, &result_mt, num_cpu);

  printf("single threaded output: %d\n", result_st);
  printf("multi threaded output : %d\n", result_mt);

  return 0;
}
