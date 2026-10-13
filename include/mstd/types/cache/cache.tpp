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
    template <typename T, Source... Sources>
    Cache<T, Sources...>::Cache(Func compute) : _compute(compute)
    {
    }

    template <typename T, Source... Sources>
    const T& Cache<T, Sources...>::get(const Sources&... sources) const
    {
        if (_value && domains::isCurrent(_epochs))
            return *_value;

        MSTD_ASSERT_NOT_HOT("Cache miss: value is not current");

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
