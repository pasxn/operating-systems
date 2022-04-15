// gcc client.c -o client.out -lpthread

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define SHMSZ 27

char SEM_NAME[] = "vik";

int main() {
    char ch;
    int shmid;
    key_t key;
    char *shm, *s;
    sem_t *mutex;
    
    // name the shared memory segment
    key = 1000;
    
    // create & initialize existing semaphore
    mutex = sem_open(SEM_NAME, 0, 0644, 0);
    if(mutex == SEM_FAILED) {
        perror("reader:unable to execute semaphore");
        sem_close (mutex);
        exit(-1);
    }

    // create a shared memory segment with this key
    shmid = shmget(key, SHMSZ, 0666);
    if(shmid < 0) {
        perror("reader:failure in shmget");
        exit(-1);
    } 

    // attach this segment to virtual memory
    shm = shmat(shmid, NULL, 0);

    // start reading
    s = shm;
    for(s = shm; *s != NULL; s++) {
        sem_wait(mutex);
        putchar(*s);
        sem_post(mutex);
    }

    // the below loop could be replaced by binary semaphore
    while(*shm != "*") {
        sleep(1);
    }
    sem_close(mutex);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    _exit(0);
}