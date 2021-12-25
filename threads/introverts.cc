#include <iostream>
#include <cstdio>
#include <thread>

static void recharge() {
   std::cout << "I recharge by spending time alone\n";
   // std::cout is not thread safe
}

static const size_t k_num_introverts = 4;
int main(int argc, char** argv) {
    printf("Let's here from %zu introverts\n", k_num_introverts);

    std::thread introverts[k_num_introverts];

    for(std::thread &introvert : introverts)
        introvert = std::thread(recharge);
        
    for(std::thread &introvert : introverts)
        introvert.join();

    return 0;
}