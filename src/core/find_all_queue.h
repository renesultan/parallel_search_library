#ifndef CORE_FIND_ALL_QUEUE_H
#define CORE_FIND_ALL_QUEUE_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

// Task structure for the work queue
template <typename C>
struct FindAllTask
{
    size_t start;
    size_t end;
    std::vector<const typename C::value_type *> *results;
    std::mutex *results_mutex;
};

// Thread-safe queue implementation
template <typename T>
class ThreadSafeQueue
{
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cond;
    bool done{false};

public:
    void push(T item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(item));
        cond.notify_one();
    }

    bool pop(T &item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (queue.empty() && !done)
        {
            cond.wait(lock);
        }
        if (queue.empty())
        {
            return false;
        }
        item = std::move(queue.front());
        queue.pop();
        return true;
    }

    void finish()
    {
        std::lock_guard<std::mutex> lock(mutex);
        done = true;
        cond.notify_all();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};

// Worker function for processing tasks
template <typename C, typename V>
void worker_function(ThreadSafeQueue<FindAllTask<C>> &task_queue, const C &container,
                     const V &value, std::atomic<bool> *terminate_flag)
{
    FindAllTask<C> task;
    while (task_queue.pop(task))
    {
        if (terminate_flag && terminate_flag->load())
        {
            break;
        }

        std::vector<const typename C::value_type *> local_results;
        for (size_t i = task.start; i < task.end; ++i)
        {
            if (terminate_flag && terminate_flag->load())
            {
                break;
            }
            if (container[i] == value)
            {
                local_results.push_back(&container[i]);
                if (terminate_flag)
                {
                    terminate_flag->store(true);
                    break;
                }
            }
        }

        if (!local_results.empty())
        {
            std::lock_guard<std::mutex> lock(*task.results_mutex);
            task.results->insert(task.results->end(), local_results.begin(), local_results.end());
        }
    }
}

// Queue-based parallel find_all implementation
template <typename C, typename V>
std::vector<const typename C::value_type *> parallel_find_all_queue(
    const C &container, const V &value, size_t num_threads, bool early_termination = false)
{

    std::vector<const typename C::value_type *> results;
    std::mutex results_mutex;
    ThreadSafeQueue<FindAllTask<C>> task_queue;
    std::vector<std::thread> threads;

    // C++11 version of unique_ptr creation
    std::atomic<bool> *raw_terminate_flag = nullptr;
    if (early_termination)
    {
        raw_terminate_flag = new std::atomic<bool>(false);
    }

    // Calculate segment size
    size_t segment_size = container.size() / (num_threads * 2); // Create more tasks than threads
    size_t current_pos = 0;

    // Create tasks
    while (current_pos < container.size())
    {
        size_t end_pos = std::min(current_pos + segment_size, container.size());
        task_queue.push(FindAllTask<C>{current_pos, end_pos, &results, &results_mutex});
        current_pos = end_pos;
    }

    // Start worker threads
    for (size_t i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(worker_function<C, V>, std::ref(task_queue),
                             std::ref(container), std::ref(value),
                             raw_terminate_flag);
    }

    // Signal completion and wait for threads
    task_queue.finish();
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Clean up
    delete raw_terminate_flag;

    return results;
}

// Timing wrapper
template <typename C, typename V>
std::pair<std::vector<const typename C::value_type *>, std::chrono::microseconds>
parallel_find_all_queue_with_timing(const C &container, const V &value,
                                    size_t num_threads, bool early_termination = false)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto results = parallel_find_all_queue(container, value, num_threads, early_termination);
    auto end = std::chrono::high_resolution_clock::now();
    return {results, std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
}

#endif // CORE_FIND_ALL_QUEUE_H