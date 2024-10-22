#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../core/find_all.h"
#include "../core/parallel_find_all.h"

void test_int_vector()
{
    std::vector<int> numbers = {1, 2, 3, 4, 2, 5, 2, 6};
    int search_value = 2;

    auto results = find_all(numbers, search_value);

    std::cout << "Found " << results.size() << " occurrences of " << search_value << std::endl;
    for (const auto &ptr : results)
    {
        std::cout << "At position: " << (ptr - &numbers[0]) << std::endl;
    }
}

void test_string_vector()
{
    std::vector<std::string> words = {"apple", "banana", "cherry", "date", "cherry", "fig"};
    std::string search_value = "cherry";

    auto results = find_all(words, search_value);

    std::cout << "Found " << results.size() << " occurrences of " << search_value << std::endl;
    for (const auto &ptr : results)
    {
        std::cout << "At position: " << (ptr - &words[0]) << std::endl;
    }
}

template <typename C, typename V>
void measure_performance(const C &container, const V &value, const std::string &container_type)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto results = find_all(container, value);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Performance for " << container_type << ":" << std::endl;
    std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
    std::cout << "Found " << results.size() << " occurrences" << std::endl
              << std::endl;
}

template <typename C, typename V>
void measure_parallel_performance(const C &container, const V &value, const std::string &container_type)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto result_pair = parallel_find_all_with_timing(container, value, 10);
    auto &results = result_pair.first;
    auto &thread_creation_time = result_pair.second;

    auto end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto search_duration = total_duration - thread_creation_time;

    std::cout << "Parallel performance for " << container_type << ":" << std::endl;
    std::cout << "Total time: " << total_duration.count() << " microseconds" << std::endl;
    std::cout << "Thread creation time: " << thread_creation_time.count() << " microseconds" << std::endl;
    std::cout << "Search time: " << search_duration.count() << " microseconds" << std::endl;
    std::cout << "Found " << results.size() << " occurrences" << std::endl
              << std::endl;
}

void find_breakeven_point()
{
    std::cout << "\n=== Break-even Point Analysis ===" << std::endl;
    std::vector<size_t> sizes = {1000, 10000, 100000, 1000000, 10000000};

    for (size_t size : sizes)
    {
        std::vector<int> numbers(size, 42);
        int search_value = 43; // Ensure we search the entire vector

        auto serial_start = std::chrono::high_resolution_clock::now();
        auto serial_results = find_all(numbers, search_value);
        auto serial_end = std::chrono::high_resolution_clock::now();
        auto serial_duration = std::chrono::duration_cast<std::chrono::microseconds>(serial_end - serial_start);

        auto parallel_start = std::chrono::high_resolution_clock::now();
        auto parallel_results = parallel_find_all(numbers, search_value, 10); // Added third argument
        auto parallel_end = std::chrono::high_resolution_clock::now();
        auto parallel_duration = std::chrono::duration_cast<std::chrono::microseconds>(parallel_end - parallel_start);

        std::cout << "Vector size: " << size << std::endl;
        std::cout << "Serial time: " << serial_duration.count() << " microseconds" << std::endl;
        std::cout << "Parallel time: " << parallel_duration.count() << " microseconds" << std::endl;
        std::cout << "Speedup: " << static_cast<double>(serial_duration.count()) / parallel_duration.count() << std::endl;
        std::cout << std::endl;
    }
}

void find_detailed_breakeven_point()
{
    std::cout << "\n=== Detailed Break-even Point Analysis ===" << std::endl;
    std::vector<size_t> sizes = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};

    for (size_t size : sizes)
    {
        std::vector<int> numbers(size, 42);
        int search_value = 43; // Ensure we search the entire vector

        auto serial_start = std::chrono::high_resolution_clock::now();
        auto serial_results = find_all(numbers, search_value);
        auto serial_end = std::chrono::high_resolution_clock::now();
        auto serial_duration = std::chrono::duration_cast<std::chrono::microseconds>(serial_end - serial_start);

        auto parallel_start = std::chrono::high_resolution_clock::now();
        auto parallel_results = parallel_find_all(numbers, search_value, 10); // Added third argument
        auto parallel_end = std::chrono::high_resolution_clock::now();
        auto parallel_duration = std::chrono::duration_cast<std::chrono::microseconds>(parallel_end - parallel_start);

        std::cout << "Vector size: " << size << std::endl;
        std::cout << "Serial time: " << serial_duration.count() << " microseconds" << std::endl;
        std::cout << "Parallel time: " << parallel_duration.count() << " microseconds" << std::endl;
        double speedup = static_cast<double>(serial_duration.count()) / parallel_duration.count();
        std::cout << "Speedup: " << speedup << std::endl;
        std::cout << (speedup > 1.0 ? "Parallel is faster" : "Serial is faster") << std::endl;
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "=== Original Tests ===" << std::endl;
    std::cout << "Testing with int vector:" << std::endl;
    test_int_vector();

    std::cout << "\nTesting with string vector:" << std::endl;
    test_string_vector();

    std::cout << "\n=== Performance Tests ===" << std::endl;
    std::vector<int> numbers(1000000, 42); // Large vector of integers
    measure_performance(numbers, 42, "Large int vector");

    std::vector<std::string> words(100000, "target"); // Large vector of strings
    measure_performance(words, std::string("target"), "Large string vector");

    std::cout << "\n=== Parallel Performance Tests ===" << std::endl;
    measure_parallel_performance(numbers, 42, "Large int vector");
    measure_parallel_performance(words, std::string("target"), "Large string vector");

    find_breakeven_point();
    find_detailed_breakeven_point();

    return 0;
}