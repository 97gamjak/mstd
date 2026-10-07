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

#ifndef __MSTD__TYPES__CACHE_HPP__
#define __MSTD__TYPES__CACHE_HPP__

#include <cstddef>
#include <functional>
#include <optional>

namespace mstd
{
    template <typename T>
    class CacheProperty;   // forward declaration

    /**
     * @brief A simple cache class that stores a value of type T and allows
     * for lazy computation of the value using a registered compute function.
     * (NOT thread-safe)
     */
    template <typename T>
    class Cache
    {
       private:
        mutable std::optional<T> _value   = std::nullopt;
        mutable size_t           _version = 0;

       public:
        Cache() = default;

        template <typename P>
        const T& get(
            const CacheProperty<P>&    source,
            std::function<T(const P&)> compute
        ) const;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__CACHE_TPP__
#include "cache.tpp"
#endif

#endif   // __MSTD__TYPES__CACHE_HPP__
