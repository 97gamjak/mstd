/*****************************************************************************
<GPL_HEADER>

    mstd library
    Copyright (C) 2025-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#ifndef __MSTD__TYPES__CACHE_TPP__
#define __MSTD__TYPES__CACHE_TPP__

#include "cache.hpp"
#include "mstd/debugging.hpp"
#include "mstd/exceptions.hpp"

namespace mstd
{
    /**
     * @brief Constructs a Cache with the given compute function.
     *
     * @param compute The compute function to use for the cache.
     */
    template <typename T, Source... Sources>
    Cache<T, Sources...>::Cache(Func compute) : _compute(compute)
    {
    }

    /**
     * @brief Retrieves the cached value, computing it if necessary.
     *
     * @param sources The sources to use for computing the cache value if it is
     * not current.
     * @return The cached value.
     */
    template <typename T, Source... Sources>
    const T& Cache<T, Sources...>::get(const Sources&... sources) const
    {
        if (_value && domains::isCurrent(_epochs))
            return *_value;

        MSTD_ASSERT_NOT_HOT("Cache miss: value is not current");

        _slow(sources...);
        return *_value;
    }

    /**
     * @brief Slow path for computing the cache value.
     *
     * This function is called when the cache value is not current. It uses
     * the compute function to update the value and the epochs.
     *
     * @param sources The sources to use for computing the cache value.
     */
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

    /**
     * @brief Constructs a BoundCache with the given compute function and
     * sources.
     *
     * @param compute The compute function to use for the cache.
     * @param sources The sources to bind to the cache.
     */
    template <typename T, Source... Sources>
    BoundCache<T, Sources...>::BoundCache(
        Func compute,
        const Sources&... sources
    )
        : _cache(compute), _sources(&sources...)
    {
    }

    /**
     * @brief Retrieves the cached value from the bound cache.
     *
     * @return The cached value.
     */
    template <typename T, Source... Sources>
    const T& BoundCache<T, Sources...>::get() const
    {
        return std::apply(
            [this](const Sources*... sources) -> const T&
            { return _cache.get(*sources...); },
            _sources
        );
    }

}   // namespace mstd

#endif   // __MSTD__TYPES__CACHE_TPP__
