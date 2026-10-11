#ifndef __MSTD__TYPES__CACHE_TPP__
#define __MSTD__TYPES__CACHE_TPP__

#include "cache.hpp"
#include "mstd/exceptions.hpp"

namespace mstd
{
    template <typename T, Source... Sources>
    Cache<T, Sources...>::Cache(Func compute) : _compute(compute)
    {
    }

    template <typename T, Source... Sources>
    const T& Cache<T, Sources...>::get(const Sources&... sources) const
    {
        if (_value && domains::isCurrent(_epochs))
            return *_value;

        _slow(sources...);
        return *_value;
    }

    template <typename T, Source... Sources>
    void Cache<T, Sources...>::_slow(const Sources&... sources) const
    {
        if (!_compute)
        {
            throw mstd::RuntimeError(
                "Cache value is not valid and no compute function is set."
            );
        }

        _value  = _compute(sources.get()...);
        _epochs = domains::now();
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__CACHE_TPP__
