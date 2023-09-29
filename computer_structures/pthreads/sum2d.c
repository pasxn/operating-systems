#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


// define a structure to hold thread-specific data
typedef struct _thread_data {
  int* a;
  int* answer;
  int col;
} thread_data;

void* sum_thread(void* arg) {
  thread_data* data = (thread_data*)arg;

  for(int i = 0; i < data->col; i++) {
    *data->answer += *(data->a + i);
  }

  pthread_exit(NULL);
}

void sum_mt(int* a, const int a_row, const int a_col, int* c) {
  *c = 0;
  int* answers[a_row];
  pthread_t thread_id[a_row];
  thread_data td[a_row];

  for (int i = 0; i < a_row; i++) {
    td[i].a = (a + (i * a_col));
    td[i].answer = answers[i];
    td[i].col = a_col;

    pthread_create(&thread_id[i], NULL, sum_thread, &td[i]);
  }

  for (int i = 0; i < a_row; i++) {
    pthread_join(thread_id[i], NULL);
  }

  for (int i = 0; i < a_row; i++) {
    *c = *answers[i];
  }  
}

void sum_st(int* a, int a_row, int a_col, int* c) {
  *c = 0;
  for(int i = 0; i < a_row; i++) {
    for(int j = 0; j < a_col; j++) {
      *c += *(a + (i * a_col) + j);
    }
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  if (argc < 2) {
    perror("comand line arguments are not entered");
    return 1;
  }

  const int row = atoi(argv[1]);
  const int col = atoi(argv[2]);
  int a[row][col];

  for (int i = 0; i < row; ++i) {
    // random numbers between 0 and 9
    for (int j = 0; j < col; ++j) {
      // a[i][j] = rand() % 10;
      a[i][j] = 1;
    }
  }

  int result_st, result_mt;

  sum_st(a, row, col, &result_st);
  sum_mt(a, row, col, &result_mt);

  printf("single threaded output: %d\n", result_st);
  printf("multi threaded output : %d\n", result_mt);

  return 0;
}
