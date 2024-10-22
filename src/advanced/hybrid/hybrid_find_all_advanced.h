#ifndef ADVANCED_HYBRID_FIND_ALL_ADVANCED_H
#define ADVANCED_HYBRID_FIND_ALL_ADVANCED_H

#include <vector>
#include "../../core/find_all.h"
#include "../../core/parallel_find_all.h"

template <typename C, typename V>
std::vector<const typename C::value_type *> hybrid_find_all_advanced(const C &c, const V &v)
{
    size_t size = c.size();

    if (size < 100000)
    {
        return parallel_find_all(c, v, 4); // Original implementation with 4 threads
    }
    else if (size < 1000000)
    {
        return parallel_find_all(c, v, 8); // Original implementation with 8 threads
    }
    else
    {
        return parallel_find_all_tp(c, v, 16); // Thread pool version with 16 threads
    }
}

#endif // ADVANCED_HYBRID_FIND_ALL_ADVANCED_H