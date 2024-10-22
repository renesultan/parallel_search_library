#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../core/find_all.h"
#include "../advanced/hybrid/hybrid_find_all_early_termination.h"

void test_hybrid_early_termination()
{
    std::cout << "\n=== Hybrid Early Termination Implementation Analysis ===" << std::endl;
    std::vector<size_t> vector_sizes = {10000, 100000, 1000000, 10000000};

    for (size_t size : vector_sizes)
    {
        std::cout << "\nVector size: " << size << std::endl;
        std::vector<int> numbers(size, 42);
        int search_value = 43;

        // Test with non-existent value
        auto start = std::chrono::high_resolution_clock::now();
        auto results = hybrid_find_all_early_termination(numbers, search_value);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  Non-existent value search:" << std::endl;
        std::cout << "    Total time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "    Found " << results.size() << " occurrences" << std::endl;

        // Test with existing value at the beginning
        numbers[0] = 43;
        start = std::chrono::high_resolution_clock::now();
        results = hybrid_find_all_early_termination(numbers, search_value);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  Existing value search (beginning):" << std::endl;
        std::cout << "    Total time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "    Found " << results.size() << " occurrences" << std::endl;

        // Test with existing value at the end
        numbers[0] = 42;
        numbers[size - 1] = 43;
        start = std::chrono::high_resolution_clock::now();
        results = hybrid_find_all_early_termination(numbers, search_value);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  Existing value search (end):" << std::endl;
        std::cout << "    Total time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "    Found " << results.size() << " occurrences" << std::endl;
    }
}

int main()
{
    test_hybrid_early_termination();
    return 0;
}