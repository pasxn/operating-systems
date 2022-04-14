#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
int X = 0;

void *writeloop (void *arg) {
    while (X < 10) {
        pthread_mutex_lock(&mutex);
        X++;
        usleep(rand()%10);
        printf("X=%d\n", X);
        pthread_mutex_unlock(&mutex);
    }
    exit (0);
}

int main(void) {
    pthread_t tid;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&tid, NULL, writeloop, NULL);
    writeloop(NULL);
    pthread_join(tid, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}