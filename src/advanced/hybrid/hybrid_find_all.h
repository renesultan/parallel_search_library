#ifndef ADVANCED_HYBRID_FIND_ALL_H
#define ADVANCED_HYBRID_FIND_ALL_H

#include <vector>
#include "../../core/find_all.h"
#include "../../core/parallel_find_all.h"

template <typename C, typename V>
std::vector<const typename C::value_type *> hybrid_find_all(const C &c, const V &v, size_t num_threads = 10)
{
    if (c.size() < 30000)
    {
        return find_all(c, v);
    }
    else
    {
        return parallel_find_all(c, v, num_threads);
    }
}

#endif // ADVANCED_HYBRID_FIND_ALL_H