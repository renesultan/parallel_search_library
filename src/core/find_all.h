#ifndef CORE_FIND_ALL_H
#define CORE_FIND_ALL_H

#include <vector>

template <typename C, typename V>
std::vector<const typename C::value_type *> find_all(const C &c, const V &v)
{
    std::vector<const typename C::value_type *> result;

    for (const auto &item : c)
    {
        if (item == v)
        {
            result.push_back(&item);
        }
    }

    return result;
}

#endif // CORE_FIND_ALL_H