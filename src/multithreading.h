#pragma once
#include <thread>
#include <vector>
#include <utility> 


template <typename Callable, typename... Args>
//void runInMultipleThreads(unsigned int num_threads, Callable && func, size_t start, size_t end, Args&&... args) {
void runInMultipleThreads(unsigned int num_threads, Callable && func, size_t start, size_t end, Args&... args) {
//void runInMultipleThreads(unsigned int num_threads, Callable && func, size_t start, size_t end, Args... args) {
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
        size_t i_start = start + i * static_cast<int>((end - start) / num_threads);
        size_t i_end   = i_start + static_cast<int>((end - start) / num_threads);
        threads.emplace_back(func, i_start, i_end, std::ref(args)...);
        //threads.emplace_back(func, i_start, i_end, std::forward<Args>(args)...);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}