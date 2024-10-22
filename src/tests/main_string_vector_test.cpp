#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../core/find_all.h"
#include "../core/parallel_find_all.h"
#include "../core/find_all_early_termination.h"
#include "../core/find_all_if_early_termination.h"

// Helper function to generate test strings
std::vector<std::string> generate_test_strings(size_t size, size_t target_position)
{
    std::vector<std::string> strings;
    strings.reserve(size);

    // Generate strings like: "string_0", "string_1", etc.
    for (size_t i = 0; i < size; ++i)
    {
        if (i == target_position)
        {
            strings.push_back("target_string");
        }
        else
        {
            strings.push_back("string_" + std::to_string(i));
        }
    }
    return strings;
}

void test_string_vector_performance()
{
    std::cout << "\n=== String Vector Performance Analysis ===\n"
              << std::endl;

    std::vector<size_t> sizes = {10000, 100000, 1000000};
    std::vector<size_t> thread_counts = {2, 4, 8, 16};
    std::string search_value = "target_string";

    for (size_t size : sizes)
    {
        std::cout << "Vector size: " << size << std::endl;

        // Test with target at beginning, middle, and end
        std::vector<size_t> positions = {0, size / 2, size - 1};
        for (size_t pos : positions)
        {
            std::cout << "\nTarget position: " << pos << std::endl;
            auto strings = generate_test_strings(size, pos);

            // 1. Basic find_all (Serial)
            {
                auto start = std::chrono::high_resolution_clock::now();
                auto results = find_all(strings, search_value);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "  Serial find_all:" << std::endl;
                std::cout << "    Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "    Found: " << results.size() << " occurrences" << std::endl;
            }

            // 2. Parallel find_all
            for (size_t thread_count : thread_counts)
            {
                auto result_pair = parallel_find_all_with_timing(strings, search_value, thread_count);
                auto &results = result_pair.first;
                auto &thread_creation_time = result_pair.second;

                std::cout << "  Parallel find_all (" << thread_count << " threads):" << std::endl;
                std::cout << "    Thread creation time: " << thread_creation_time.count() << " microseconds" << std::endl;
                std::cout << "    Found: " << results.size() << " occurrences" << std::endl;
            }

            // 3. Early termination find_all
            {
                std::atomic<bool> terminate_flag(false);
                auto start = std::chrono::high_resolution_clock::now();
                auto results = find_all_et(strings, search_value, terminate_flag);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "  Serial early termination find_all:" << std::endl;
                std::cout << "    Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "    Found: " << results.size() << " occurrences" << std::endl;
            }

            // 4. Parallel early termination find_all
            for (size_t thread_count : thread_counts)
            {
                auto start = std::chrono::high_resolution_clock::now();
                auto results = parallel_find_all_et(strings, search_value, thread_count);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "  Parallel early termination find_all (" << thread_count << " threads):" << std::endl;
                std::cout << "    Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "    Found: " << results.size() << " occurrences" << std::endl;
            }

            // 5. Find_all_if with string length predicate
            {
                auto target_length = search_value.length();
                auto length_pred = [target_length](const std::string &s)
                { return s.length() == target_length; };

                std::atomic<bool> terminate_flag(false);
                auto start = std::chrono::high_resolution_clock::now();
                auto results = find_all_if_et(strings, length_pred, terminate_flag);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "  Serial find_all_if (length predicate):" << std::endl;
                std::cout << "    Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "    Found: " << results.size() << " occurrences" << std::endl;
            }

            // 6. Parallel find_all_if with string length predicate
            for (size_t thread_count : thread_counts)
            {
                auto target_length = search_value.length();
                auto length_pred = [target_length](const std::string &s)
                { return s.length() == target_length; };

                auto start = std::chrono::high_resolution_clock::now();
                auto results = parallel_find_all_if_et(strings, length_pred, thread_count);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "  Parallel find_all_if (length predicate) (" << thread_count << " threads):" << std::endl;
                std::cout << "    Time: " << duration.count() << " microseconds" << std::endl;
                std::cout << "    Found: " << results.size() << " occurrences" << std::endl;
            }
        }
        std::cout << "\n";
    }
}

int main()
{
    test_string_vector_performance();
    return 0;
}