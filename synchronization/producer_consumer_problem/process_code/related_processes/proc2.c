// gcc proc2.c -lpthread

#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv) {
    int fd, i, count=0, nloop=10, int zero=0; 
    int *ptr;

    sem_t mutex;
    sem_t full;
    sem_t empty;

    // open a file and map it into memory
    fd = open("log.txt", O_RDWR|O_CREAT, S_IRWXU);
    write(fd,&zero, sizeof(int ));
    ptr = mmap (NULL,sizeof(int) , PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd) ;

    // create, initialize mutex semaphore*  
    if(sem_init(&mutex, 1, 1)< 0) {
        perror("semaphore initilization");
        exit (0);
    }   
    
    // create, initialize counting semaphore
    if(sem_init(&full, 1, 12)< 0) {
        perror("semaphore initilization");
        exit (0);
    }

    // create, initialize counting semaphore
    if(sem_init(&empty, 1, 0)< 0) {
        perror("semaphore initilization");
        exit (0);
    }

    *ptr = 11;
    if(fork() == 0) { // child process -- -- consumer
        for(i = 0; i<nloop; i++) {
            sem_wait(&empty);
            sem_wait(&mutex);
            printf ("child: %d\n", (*ptr)++);
            sem_post(&mutex);
            sem_post(&full);
        }
        exit(0);
    } else { // parent process -- producer
        for(i = 0; i<nloop; i++) {
            sem_wait(&full);
            sem_wait(&mutex);
            printf("parent: %d\n", (*ptr)++);
            sem_post(&mutex);
            sem_post(&empty);
        }        
    }
    exit(0);
}