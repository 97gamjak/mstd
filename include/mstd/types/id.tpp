#ifndef __MSTD__TYPES__ID_TPP__
#define __MSTD__TYPES__ID_TPP__

#include <atomic>

#include "id.hpp"

namespace mstd
{

    /**
     * @brief Constructs an Id with the given value.
     *
     * @param v The initial value of the Id.
     */
    template <typename T, typename Tag>
    Id<T, Tag>::Id(T v) : _value(v)
    {
    }

    /**
     * @brief Returns the next Id by incrementing the current value.
     *
     * @return The next Id.
     */
    template <typename T, typename Tag>
    Id<T, Tag> Id<T, Tag>::next()
    {
        static std::atomic<T> value{0};
        return Id<T, Tag>(++value);
    }

}   // namespace mstd

#endif   // __MSTD__TYPES__ID_TPP__
