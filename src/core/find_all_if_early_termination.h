#ifndef CORE_FIND_ALL_IF_EARLY_TERMINATION_H
#define CORE_FIND_ALL_IF_EARLY_TERMINATION_H

#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <functional>

template <typename C, typename Pred>
std::vector<const typename C::value_type *> find_all_if_et(const C &c, Pred pred, std::atomic<bool> &terminate_flag)
{
    std::vector<const typename C::value_type *> result;

    for (const auto &item : c)
    {
        if (terminate_flag)
        {
            break; // Early termination requested
        }

        if (pred(item))
        {
            result.push_back(&item);
            terminate_flag = true; // Signal other threads to terminate
            break;
        }
    }

    return result;
}

// Helper function for parallel segments with early termination
template <typename C, typename Pred>
void find_all_if_segment_et(const C &c, Pred pred, size_t start, size_t end,
                            std::vector<const typename C::value_type *> &result,
                            std::mutex &result_mutex, std::atomic<bool> &terminate_flag)
{
    std::vector<const typename C::value_type *> local_result;

    for (size_t i = start; i < end && !terminate_flag; ++i)
    {
        if (pred(c[i]))
        {
            local_result.push_back(&c[i]);
            terminate_flag = true; // Signal other threads to terminate
            break;
        }
    }

    if (!local_result.empty())
    {
        std::lock_guard<std::mutex> lock(result_mutex);
        result.insert(result.end(), local_result.begin(), local_result.end());
    }
}

// N-way parallel version with early termination
template <typename C, typename Pred>
std::vector<const typename C::value_type *> parallel_find_all_if_et(const C &c, Pred pred, size_t num_threads)
{
    std::vector<const typename C::value_type *> result;
    std::vector<std::thread> threads;
    std::mutex result_mutex;
    std::atomic<bool> terminate_flag(false);

    size_t segment_size = c.size() / num_threads;
    size_t remainder = c.size() % num_threads;

    size_t start = 0;
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t end = start + segment_size + (i < remainder ? 1 : 0);
        threads.emplace_back(find_all_if_segment_et<C, Pred>, std::ref(c), pred,
                             start, end, std::ref(result), std::ref(result_mutex),
                             std::ref(terminate_flag));
        start = end;
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return result;
}

#endif // CORE_FIND_ALL_IF_EARLY_TERMINATION_H