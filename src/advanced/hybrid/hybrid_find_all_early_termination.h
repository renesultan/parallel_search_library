#ifndef ADVANCED_HYBRID_FIND_ALL_EARLY_TERMINATION_H
#define ADVANCED_HYBRID_FIND_ALL_EARLY_TERMINATION_H

#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include "../../core/find_all.h"
#include "../../core/parallel_find_all.h"

template <typename C, typename V>
void find_all_segment_et(const C &c, const V &v, size_t start, size_t end,
                         std::vector<const typename C::value_type *> &result,
                         std::mutex &result_mutex, std::atomic<bool> &found)
{
    for (size_t i = start; i < end && !found; ++i)
    {
        if (c[i] == v)
        {
            std::lock_guard<std::mutex> lock(result_mutex);
            result.push_back(&c[i]);
            found = true;
            return;
        }
    }
}

template <typename C, typename V>
std::vector<const typename C::value_type *> parallel_find_all_et(const C &c, const V &v, size_t num_threads)
{
    std::vector<const typename C::value_type *> result;
    std::vector<std::thread> threads;
    std::mutex result_mutex;
    std::atomic<bool> found(false);

    size_t segment_size = c.size() / num_threads;
    size_t remainder = c.size() % num_threads;

    size_t start = 0;
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t end = start + segment_size + (i < remainder ? 1 : 0);
        threads.emplace_back(find_all_segment_et<C, V>, std::ref(c), std::ref(v),
                             start, end, std::ref(result), std::ref(result_mutex), std::ref(found));
        start = end;
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return result;
}

template <typename C, typename V>
std::vector<const typename C::value_type *> hybrid_find_all_early_termination(const C &c, const V &v)
{
    size_t size = c.size();

    if (size < 100000)
    {
        return parallel_find_all_et(c, v, 4); // Use 4 threads for small vectors
    }
    else if (size < 1000000)
    {
        return parallel_find_all_et(c, v, 8); // Use 8 threads for medium vectors
    }
    else
    {
        return parallel_find_all_et(c, v, 16); // Use 16 threads for large vectors
    }
}

#endif // ADVANCED_HYBRID_FIND_ALL_EARLY_TERMINATION_H