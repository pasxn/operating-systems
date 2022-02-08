#include <iostream>
#include <cstdio>
#include <thread>

static void greet(int id) {
    static int n = 0;
    for(size_t i = 0; i < id; i++) {
        printf("Greeter# : %d, say hello!\n", n++);

        struct timespec ts = {0, random() % 1000000000}; 
        nanosleep(&ts, NULL);       
    }
    printf("All hellos done from %d\n", id);
}

static const size_t k_num = 4;
int main(int argc, char** argv) {
    std::cout << "greeting program\n";

    std::thread greeters[k_num ];

    for(size_t i = 0; i<k_num; i++)
        greeters[i] = std::thread(greet, 3); // i+1
    
    for(size_t i = 0; i<k_num; i++)
        greeters[i].join();

    std::cout << "Done!\n";
    return 0;
}