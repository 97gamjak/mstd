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
