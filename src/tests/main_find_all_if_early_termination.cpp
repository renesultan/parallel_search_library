#include <iostream>
#include <vector>
#include <chrono>
#include "../core/find_all_if_early_termination.h"

void test_find_all_if_early_termination()
{
    std::cout << "\n=== Find All If Early Termination Analysis ===" << std::endl;
    std::vector<size_t> vector_sizes = {10000, 100000, 1000000, 10000000};
    std::vector<size_t> thread_counts = {2, 4, 8, 16};

    // Test cases:
    // 1. Search for first even number (will find immediately)
    // 2. Search for first number divisible by vector_size/2 (middle)
    // 3. Search for first number divisible by vector_size (end)

    for (size_t size : vector_sizes)
    {
        std::cout << "\nVector size: " << size << std::endl;
        std::vector<int> numbers(size);

        // Fill vector with consecutive numbers
        for (size_t i = 0; i < size; ++i)
        {
            numbers[i] = i + 1;
        }

        // Case 1: First even number (position 1)
        {
            auto pred = [](int x)
            { return x % 2 == 0; };
            std::cout << "\n  Looking for first even number:" << std::endl;

            // Serial version
            std::atomic<bool> terminate_flag(false);
            auto start = std::chrono::high_resolution_clock::now();
            auto results = find_all_if_et(numbers, pred, terminate_flag);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "    Serial version:" << std::endl;
            std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
            std::cout << "      Found: " << results.size() << " occurrences" << std::endl;

            // Parallel versions
            for (size_t thread_count : thread_counts)
            {
                start = std::chrono::high_resolution_clock::now();
                results = parallel_find_all_if_et(numbers, pred, thread_count);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "    Parallel version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << results.size() << " occurrences" << std::endl;
            }
        }

        // Case 2: Middle position (number divisible by size/2)
        {
            auto mid_value = size / 2;
            auto pred = [mid_value](int x)
            { return x % mid_value == 0; };
            std::cout << "\n  Looking for first number divisible by " << mid_value << ":" << std::endl;

            // Serial version
            std::atomic<bool> terminate_flag(false);
            auto start = std::chrono::high_resolution_clock::now();
            auto results = find_all_if_et(numbers, pred, terminate_flag);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "    Serial version:" << std::endl;
            std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
            std::cout << "      Found: " << results.size() << " occurrences" << std::endl;

            // Parallel versions
            for (size_t thread_count : thread_counts)
            {
                start = std::chrono::high_resolution_clock::now();
                results = parallel_find_all_if_et(numbers, pred, thread_count);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "    Parallel version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << results.size() << " occurrences" << std::endl;
            }
        }

        // Case 3: End position (number divisible by size)
        {
            auto pred = [size](int x)
            { return x % size == 0; };
            std::cout << "\n  Looking for first number divisible by " << size << ":" << std::endl;

            // Serial version
            std::atomic<bool> terminate_flag(false);
            auto start = std::chrono::high_resolution_clock::now();
            auto results = find_all_if_et(numbers, pred, terminate_flag);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "    Serial version:" << std::endl;
            std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
            std::cout << "      Found: " << results.size() << " occurrences" << std::endl;

            // Parallel versions
            for (size_t thread_count : thread_counts)
            {
                start = std::chrono::high_resolution_clock::now();
                results = parallel_find_all_if_et(numbers, pred, thread_count);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "    Parallel version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << results.size() << " occurrences" << std::endl;
            }
        }
    }
}

int main()
{
    test_find_all_if_early_termination();
    return 0;
}