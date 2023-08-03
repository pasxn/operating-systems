#include <iostream>
#include <cstdio>
#include <thread>
#include <mutex>

void my_sleep() {
    struct timespec ts = {0, random() % 1000000000}; 
    nanosleep(&ts, NULL);  
}

static void ticket_agent(int id, uint32_t &remaining_tickets, std::mutex &tickets_lock) {
    while(true) {
        tickets_lock.lock();
        
        if(remaining_tickets == 0) break;

        my_sleep();
        remaining_tickets--;

        printf("Agent #%d sold a ticket!, %d tickets remaining\n", id, remaining_tickets);
    }
    tickets_lock.unlock();
}

static const size_t t_num = 10;
int main(int argc, char** argv) {

    std::mutex tickets_lock;

    std::thread agents[t_num];
    int remaining_tickets = 250;

    for(size_t i = 0; i<t_num; i++)
        agents[i] = std::thread(ticket_agent, 100 + i, std::ref(remaining_tickets),  std::ref(tickets_lock)); // sending by reference
    
    for(size_t i = 0; i<t_num; i++)
        agents[i].join();

    std::cout << "Done issuing tickets!\n";
    return 0;
}