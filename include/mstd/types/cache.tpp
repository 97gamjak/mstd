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
#include "mstd/exceptions.hpp"

namespace mstd
{

    /**
     * @brief Checks if there are duplicate domains among the sources.
     *
     * @tparam I The index of the current source.
     * @tparam J The indices of the other sources to compare against.
     * @return true if there is a duplicate domain, false otherwise.
     */
    template <typename T, Source... Sources>
    template <std::size_t I, std::size_t... J>
    constexpr bool Cache<T, Sources...>::_dupImpl(std::index_sequence<J...>)
    {
        return ((J < I && std::is_same_v<Dom<J>, Dom<I>>) || ...);
    }

    /**
     * @brief Constructs a Cache object with the given compute function.
     *
     * @param compute The compute function used to lazily compute the cached
     * value.
     */
    template <typename T, Source... Sources>
    Cache<T, Sources...>::Cache(Func compute) : _compute(compute)
    {
    }

    /**
     * @brief Retrieves the cached value, computing it if necessary.
     *
     * @return The cached value of type T.
     */
    template <typename T, Source... Sources>
    const T& Cache<T, Sources...>::get(const Sources&... sources) const
    {
        if (_isUpToDate(sources...) && _value)
            return *_value;

        _slow(sources...);

        return *_value;
    }

    /**
     * @brief Handles the slow path when the cached value is not up-to-date.
     *
     * @param sources The sources to use for recomputing the cached value.
     */
    template <typename T, Source... Sources>
    void Cache<T, Sources...>::_slow(const Sources&... sources) const
    {
        if (_compute)
        {
            _value    = _compute(sources.get()...);
            _versions = {sources.version()...};
        }
        else
        {
            throw mstd::RuntimeError(
                "Cache value is not valid and no compute function is set."
            );
        }
    }

    /**
     * @brief Checks if the cached value is up-to-date with the given
     * properties.
     *
     * @return true if the cached value is up-to-date, false otherwise.
     */
    template <typename T, Source... Sources>
    template <size_t... Is>
    bool Cache<T, Sources...>::_isUpToDate(const Sources&... properties) const
    {
        return (
            (_isDup<Is> || _versions[Is] == GlobalEpoch<Dom<Is>>::now()) && ...
        );
    }

}   // namespace mstd

#endif   // __MSTD__TYPES__CACHE_TPP__
