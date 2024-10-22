#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../advanced/find_all_if/find_all_if.h"

void test_find_all_if()
{
    std::cout << "\n=== Find All If Analysis ===" << std::endl;
    std::vector<size_t> vector_sizes = {10000, 100000, 1000000, 10000000};
    std::vector<size_t> thread_counts = {2, 4, 8, 10, 16, 32};

    auto is_even = [](int x)
    { return x % 2 == 0; };

    for (size_t size : vector_sizes)
    {
        std::cout << "\nVector size: " << size << std::endl;
        std::vector<int> numbers(size);
        for (size_t i = 0; i < size; ++i)
        {
            numbers[i] = i;
        }

        // Serial version
        auto start = std::chrono::high_resolution_clock::now();
        auto results = find_all_if(numbers, is_even);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  Serial version:" << std::endl;
        std::cout << "    Total time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "    Found " << results.size() << " even numbers" << std::endl;

        // Parallel versions
        for (size_t thread_count : thread_counts)
        {
            start = std::chrono::high_resolution_clock::now();
            results = parallel_find_all_if(numbers, is_even, thread_count);
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "  Parallel version (" << thread_count << " threads):" << std::endl;
            std::cout << "    Total time: " << duration.count() << " microseconds" << std::endl;
            std::cout << "    Found " << results.size() << " even numbers" << std::endl;
        }
    }
}

int main()
{
    test_find_all_if();
    return 0;
}