#include <stdio.h>
#include <pthread.h>

static void* recharge(void* args) {
    static int num = 0;
    printf("I recharge by spending time alone: [%d]\n", num++);
    return NULL;
}

static const size_t k_num_introverts = 4;
int main() {
    printf("Let's here from %zu introverts\n", k_num_introverts);

    pthread_t introverts[k_num_introverts];

    // seeing thread id
    for(size_t i = 0; i < k_num_introverts; i++) 
        printf("%ld\n", introverts[i]);

    for(size_t i = 0; i < k_num_introverts; i++) 
        pthread_create(&introverts[i], NULL, recharge, NULL);

    for(size_t i = 0; i < k_num_introverts; i++) 
        pthread_join(introverts[i], NULL);

    printf("Everyone is recharged\n");

    // seeing thread id
    for(size_t i = 0; i < k_num_introverts; i++) 
        printf("%ld\n", introverts[i]);
        
    return 0;
}