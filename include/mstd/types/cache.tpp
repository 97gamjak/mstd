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
     * @brief Retrieves the cached value, computing it if necessary.
     *
     * @return The cached value of type T.
     */
    template <typename T>
    template <typename P>
    const T& Cache<T>::get(
        const CacheProperty<P>&    source,
        std::function<T(const P&)> compute
    ) const
    {
        if (!_value.has_value() || _version != source.version())
        {
            _value   = compute(source.get());
            _version = source.version();
            return *_value;
        }

        throw mstd::RuntimeError(
            "Cache value is not valid and no compute function is set."
        );
    }

}   // namespace mstd

#endif   // __MSTD__TYPES__CACHE_TPP__
