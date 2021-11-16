#include <iostream>
#include <vector>
#include <functional>

int add(int x, int y) {
    return x+y;
}

void modify_vec(std::vector<int> &vec, int val, std::function<int(int, int)>op) {
    for(auto &v: vec)
        v = op(v, val);
}

void modify_vec(std::vector<int> &vec, int val, std::function<int(int)>op) {
    for(auto &v: vec)
        v = op(v);
}

int main() {  
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> vec2 = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::vector<int> vec3 = {100, 200, 300, 400, 500, 600, 700, 800, 900};
    int val = 10;

    modify_vec(vec, val, add);
    for(auto &v: vec)
        std::cout << v << std::endl;

    modify_vec(vec2, val, [](int x, int y){return x/y;});
    for(auto &v: vec2)
        std::cout << v << std::endl;

    modify_vec(vec3, val, [val](int x){return x/val;});
    for(auto &v: vec3)
        std::cout << v << std::endl;    
}