#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../core/find_all.h"
#include "../core/parallel_find_all.h"
#include "../advanced/hybrid/hybrid_find_all.h"

void test_n_way_parallelism_with_sizes()
{
    std::cout << "\n=== N-way Parallelism Analysis with Different Vector Sizes ===" << std::endl;
    std::vector<size_t> thread_counts = {2, 4, 8, 10, 16, 32};
    std::vector<size_t> vector_sizes = {10000, 100000, 1000000, 10000000};

    for (size_t size : vector_sizes)
    {
        std::cout << "\nVector size: " << size << std::endl;
        std::vector<int> numbers(size, 42);
        int search_value = 43;

        for (size_t thread_count : thread_counts)
        {
            // Original implementation
            auto start = std::chrono::high_resolution_clock::now();
            auto result_pair = parallel_find_all_with_timing(numbers, search_value, thread_count);
            auto &results = result_pair.first;
            auto &thread_creation_time = result_pair.second;
            auto end = std::chrono::high_resolution_clock::now();
            auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            auto search_duration = total_duration - thread_creation_time;

            std::cout << "  Thread count: " << thread_count << " (Original)" << std::endl;
            std::cout << "    Total time: " << total_duration.count() << " microseconds" << std::endl;
            std::cout << "    Thread creation time: " << thread_creation_time.count() << " microseconds" << std::endl;
            std::cout << "    Search time: " << search_duration.count() << " microseconds" << std::endl;
            std::cout << "    Found " << results.size() << " occurrences" << std::endl;

            // Thread pool implementation
            start = std::chrono::high_resolution_clock::now();
            result_pair = parallel_find_all_with_timing_tp(numbers, search_value, thread_count);
            results = result_pair.first;
            thread_creation_time = result_pair.second;
            end = std::chrono::high_resolution_clock::now();
            total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            search_duration = total_duration - thread_creation_time;

            std::cout << "  Thread count: " << thread_count << " (Thread Pool)" << std::endl;
            std::cout << "    Total time: " << total_duration.count() << " microseconds" << std::endl;
            std::cout << "    Thread creation time: " << thread_creation_time.count() << " microseconds" << std::endl;
            std::cout << "    Search time: " << search_duration.count() << " microseconds" << std::endl;
            std::cout << "    Found " << results.size() << " occurrences" << std::endl;
        }
    }
}

int main()
{
    test_n_way_parallelism_with_sizes();
    return 0;
}