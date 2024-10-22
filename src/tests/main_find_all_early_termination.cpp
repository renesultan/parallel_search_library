#include <iostream>
#include <vector>
#include <chrono>
#include "../core/find_all_early_termination.h"

void test_early_termination()
{
    std::cout << "\n=== Early Termination Find All Analysis ===" << std::endl;
    std::vector<size_t> vector_sizes = {10000, 100000, 1000000, 10000000};
    std::vector<size_t> thread_counts = {2, 4, 8, 16};

    for (size_t size : vector_sizes)
    {
        std::cout << "\nVector size: " << size << std::endl;
        std::vector<int> numbers(size, 42);

        // Test cases for different positions
        std::vector<size_t> positions = {0, size / 2, size - 1};

        for (size_t pos : positions)
        {
            numbers[pos] = 43; // Place search value at different positions
            std::cout << "\n  Value at position: " << pos << std::endl;

            // Test serial version
            std::atomic<bool> terminate_flag(false);
            auto start = std::chrono::high_resolution_clock::now();
            auto results = find_all_et(numbers, 43, terminate_flag);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "    Serial version:" << std::endl;
            std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
            std::cout << "      Found: " << results.size() << " occurrences" << std::endl;

            // Test parallel versions
            for (size_t thread_count : thread_counts)
            {
                start = std::chrono::high_resolution_clock::now();
                results = parallel_find_all_et(numbers, 43, thread_count);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "    Parallel version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << results.size() << " occurrences" << std::endl;
            }

            numbers[pos] = 42; // Reset for next test
        }
    }
}

int main()
{
    test_early_termination();
    return 0;
}
