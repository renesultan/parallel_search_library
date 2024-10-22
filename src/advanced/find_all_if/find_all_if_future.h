#ifndef ADVANCED_FIND_ALL_IF_FUTURE_H
#define ADVANCED_FIND_ALL_IF_FUTURE_H

#include <vector>
#include <functional>
#include <future>
#include <algorithm>
#include <atomic>

template <typename C, typename Pred>
std::vector<const typename C::value_type *> find_all_if_segment_future(const C &c, Pred pred, size_t start, size_t end, std::atomic<bool> &found)
{
    std::vector<const typename C::value_type *> result;
    for (size_t i = start; i < end && !found; ++i)
    {
        if (pred(c[i]))
        {
            result.push_back(&c[i]);
            found = true;
            break;
        }
    }
    return result;
}

template <typename C, typename Pred>
std::vector<const typename C::value_type *> parallel_find_all_if_future(const C &c, Pred pred, size_t num_threads)
{
    std::vector<std::future<std::vector<const typename C::value_type *>>> futures;
    std::atomic<bool> found(false);

    size_t segment_size = c.size() / num_threads;
    size_t remainder = c.size() % num_threads;

    size_t start = 0;
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t end = start + segment_size + (i < remainder ? 1 : 0);
        futures.push_back(std::async(std::launch::async, find_all_if_segment_future<C, Pred>,
                                     std::ref(c), pred, start, end, std::ref(found)));
        start = end;
    }

    std::vector<const typename C::value_type *> result;
    for (auto &future : futures)
    {
        auto segment_result = future.get();
        result.insert(result.end(), segment_result.begin(), segment_result.end());
        if (!segment_result.empty())
        {
            break; // Early termination
        }
    }

    return result;
}

#endif // ADVANCED_FIND_ALL_IF_FUTURE_H