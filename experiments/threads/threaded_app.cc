#include <iostream>
#include <cstdio>
#include <thread>
#include <map>
#include <string>
#include <chrono>

void refresh_forecast(std::map<std::string, int> forcast_map) {
    while(true) {
        for(auto &item : forcast_map) {
        item.second++;
        std::cout << item.first << " : " << item.second << std::endl;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);
    }
}

int main(int argc, char** argv) {
    std::map<std::string, int> forcast_map = {
        {"New York", 15},
        {"Mumbai", 15},
        {"Berlin", 15}
    };

    std::thread bg_worker(refresh_forecast, forcast_map);
    bg_worker.join();
}