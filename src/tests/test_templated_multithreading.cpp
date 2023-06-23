#include <thread>
#include <vector>
#include <iostream>

#include "../multithreading.h"



//template<typename T>
void fill_worker(size_t i_start, size_t i_end, std::vector<float>& data) {
    for (size_t i = i_start; i < i_end; i++) {
        data[i] = static_cast<float>(i);
    }
}

int main() {
    std::vector<float> data;
    //data.resize(10'000'000);
    data.resize(10);

    runInMultipleThreads(3, fill_worker, 0, 10, data);

    std::cout << data[2] << std::endl;


    return 0;
}