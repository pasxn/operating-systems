#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[5];

void * doSomeThing(void *arg){
    unsigned long i = 0;
    pthread_t id = pthread_self();

    sleep(2);
    if(pthread_equal(id,tid[0])){
        printf("\n Thread zero processing\n");
    }else if(pthread_equal(id,tid[1])){
        printf("\n Thread One processing\n");
    }else if(pthread_equal(id,tid[2])){
        printf("\n Thread Two processing\n");
    }else if(pthread_equal(id,tid[3])){
        printf("\n Thread Three processing\n");
    }else{
        printf("\n Thread Four processing\n");
    }
    for(i=0; i<(0xFFFFFFFF);i++)
            ;
    return NULL;
}

int main(void){
    int i = 0;
    int err;
    //[   ][   ][   ][   ][   ]
    //          ^
    while(i < 5){
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread %d created successfully\n", i);
            
        i++;
    }
    
    int j=0;
    char arr[10];
    for(j=0;j<5;j++){
        pthread_join(tid[j],(void **)(&arr));
    }
    //sleep(15);
    printf("\n main process exiting\n");
    return 0;
}
