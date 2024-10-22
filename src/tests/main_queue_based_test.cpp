#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../core/find_all_queue.h"

void test_queue_based_implementation()
{
    std::cout << "\n=== Queue-Based Implementation Analysis ===" << std::endl;
    std::vector<size_t> vector_sizes = {10000, 100000, 1000000, 10000000};
    std::vector<size_t> thread_counts = {2, 4, 8, 16};

    // Test both integer and string vectors
    for (size_t size : vector_sizes)
    {
        std::cout << "\nVector size: " << size << std::endl;

        // Integer vector tests
        std::cout << "\nTesting with integers:" << std::endl;
        std::vector<int> numbers(size, 42);

        // Test with target at beginning, middle, and end
        std::vector<size_t> positions = {0, size / 2, size - 1};
        for (size_t pos : positions)
        {
            if (pos < numbers.size())
            {
                numbers[pos] = 43;
            }

            std::cout << "\n  Target position: " << pos << std::endl;

            // Test without early termination
            for (size_t thread_count : thread_counts)
            {
                auto result = parallel_find_all_queue_with_timing(numbers, 43, thread_count, false);
                std::cout << "    Regular version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << result.second.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << result.first.size() << " occurrences" << std::endl;
            }

            // Test with early termination
            for (size_t thread_count : thread_counts)
            {
                auto result = parallel_find_all_queue_with_timing(numbers, 43, thread_count, true);
                std::cout << "    Early termination version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << result.second.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << result.first.size() << " occurrences" << std::endl;
            }

            numbers[pos] = 42; // Reset for next test
        }

        // String vector tests
        std::cout << "\nTesting with strings:" << std::endl;
        std::vector<std::string> strings(size, "regular");
        std::string target = "target";

        for (size_t pos : positions)
        {
            if (pos < strings.size())
            {
                strings[pos] = target;
            }

            std::cout << "\n  Target position: " << pos << std::endl;

            // Test without early termination
            for (size_t thread_count : thread_counts)
            {
                auto result = parallel_find_all_queue_with_timing(strings, target, thread_count, false);
                std::cout << "    Regular version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << result.second.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << result.first.size() << " occurrences" << std::endl;
            }

            // Test with early termination
            for (size_t thread_count : thread_counts)
            {
                auto result = parallel_find_all_queue_with_timing(strings, target, thread_count, true);
                std::cout << "    Early termination version (" << thread_count << " threads):" << std::endl;
                std::cout << "      Time: " << result.second.count() << " microseconds" << std::endl;
                std::cout << "      Found: " << result.first.size() << " occurrences" << std::endl;
            }

            strings[pos] = "regular"; // Reset for next test
        }
    }
}

int main()
{
    test_queue_based_implementation();
    return 0;
}