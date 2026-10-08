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

#include <functional>

#include "cache.hpp"
#include "mstd/exceptions.hpp"

namespace mstd
{
    /**
     * @brief Retrieves the cached value, computing it if necessary.
     *
     * @return The cached value of type T.
     */
    template <typename T>
    template <typename F, typename... Ps>
    const T& Cache<T>::get(
        F&& compute,
        const CacheProperty<Ps>&... sources
    ) const
    {
        if (!_value.has_value() || !_isUpToDate(sources...))
        {
            _value = std::invoke(std::forward<F>(compute));
            _stamps.assign({sources.stamp()...});
            return *_value;
        }

        throw mstd::RuntimeError(
            "Cache value is not valid and no compute function is set."
        );
    }

    /**
     * @brief Checks if the cached value is up-to-date with the given
     * properties.
     *
     * @return true if the cached value is up-to-date, false otherwise.
     */
    template <typename T>
    template <typename... Ps>
    bool Cache<T>::_isUpToDate(const CacheProperty<Ps>&... properties) const
    {
        if (!_value || _stamps.size() != sizeof...(Ps))
            return false;

        std::size_t i = 0;
        return ((_stamps[i++] == properties.stamp()) && ...);
    }

}   // namespace mstd

#endif   // __MSTD__TYPES__CACHE_TPP__
