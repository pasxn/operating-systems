#include <stdio.h>
#include <pthread.h>

static const char* people[] = {"Jack", "Luis", "John", "Lisa", "Paul", "Jeff", "Noone"};

static const size_t num_people = ( sizeof(people)/sizeof(people[0]) ) - 1;

static void* meetup(void* args) {
    const char* name = args;
    printf("Hey, I'm %s\n", name);
    return NULL;
}

int main() {
    pthread_t people_arr[num_people];

    for(size_t i = 0; i < num_people; i++) 
        pthread_create(&people_arr[i], NULL, meetup, (void*)people[i]);

    for(size_t i = 0; i < num_people; i++) 
        pthread_join(people_arr[i], NULL);
        
    return 0;
}