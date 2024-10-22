#ifndef CORE_PARALLEL_FIND_ALL_H
#define CORE_PARALLEL_FIND_ALL_H

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include "thread_pool.h"

// Helper function for standard parallel implementation
template <typename C, typename V>
void find_all_segment(const C &c, const V &v, size_t start, size_t end,
                      std::vector<const typename C::value_type *> &result,
                      std::mutex &result_mutex)
{
    std::vector<const typename C::value_type *> local_result;

    for (size_t i = start; i < end; ++i)
    {
        if (c[i] == v)
        {
            local_result.push_back(&c[i]);
        }
    }

    std::lock_guard<std::mutex> lock(result_mutex);
    result.insert(result.end(), local_result.begin(), local_result.end());
}

// Helper function for thread pool implementation
template <typename C, typename V>
void find_all_segment_tp(const C &c, const V &v, size_t start, size_t end,
                         std::vector<const typename C::value_type *> &result,
                         std::mutex &result_mutex)
{
    std::vector<const typename C::value_type *> local_result;

    for (size_t i = start; i < end; ++i)
    {
        if (c[i] == v)
        {
            local_result.push_back(&c[i]);
        }
    }

    std::lock_guard<std::mutex> lock(result_mutex);
    result.insert(result.end(), local_result.begin(), local_result.end());
}

template <typename C, typename V>
std::pair<std::vector<const typename C::value_type *>, std::chrono::microseconds>
parallel_find_all_with_timing(const C &c, const V &v, size_t num_threads)
{
    std::vector<const typename C::value_type *> result;
    std::vector<std::thread> threads;
    std::mutex result_mutex;

    size_t segment_size = c.size() / num_threads;
    size_t remainder = c.size() % num_threads;

    auto thread_creation_start = std::chrono::high_resolution_clock::now();

    size_t start = 0;
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t end = start + segment_size + (i < remainder ? 1 : 0);
        threads.emplace_back(find_all_segment<C, V>, std::ref(c), std::ref(v),
                             start, end, std::ref(result), std::ref(result_mutex));
        start = end;
    }

    auto thread_creation_end = std::chrono::high_resolution_clock::now();
    auto thread_creation_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        thread_creation_end - thread_creation_start);

    for (auto &thread : threads)
    {
        thread.join();
    }

    return {result, thread_creation_duration};
}

template <typename C, typename V>
std::vector<const typename C::value_type *> parallel_find_all(const C &c, const V &v, size_t num_threads)
{
    return parallel_find_all_with_timing(c, v, num_threads).first;
}

template <typename C, typename V>
std::pair<std::vector<const typename C::value_type *>, std::chrono::microseconds>
parallel_find_all_with_timing_tp(const C &c, const V &v, size_t num_threads)
{
    std::vector<const typename C::value_type *> result;
    std::mutex result_mutex;

    size_t segment_size = c.size() / num_threads;
    size_t remainder = c.size() % num_threads;

    auto thread_creation_start = std::chrono::high_resolution_clock::now();

    ThreadPool pool(num_threads);
    std::vector<std::future<void>> futures;

    size_t start = 0;
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t end = start + segment_size + (i < remainder ? 1 : 0);
        futures.emplace_back(
            pool.enqueue(find_all_segment_tp<C, V>, std::ref(c), std::ref(v),
                         start, end, std::ref(result), std::ref(result_mutex)));
        start = end;
    }

    auto thread_creation_end = std::chrono::high_resolution_clock::now();
    auto thread_creation_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        thread_creation_end - thread_creation_start);

    for (auto &future : futures)
    {
        future.get();
    }

    return {result, thread_creation_duration};
}

template <typename C, typename V>
std::vector<const typename C::value_type *> parallel_find_all_tp(const C &c, const V &v, size_t num_threads)
{
    return parallel_find_all_with_timing_tp(c, v, num_threads).first;
}

#endif // CORE_PARALLEL_FIND_ALL_H