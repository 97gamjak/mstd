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

#ifndef __MSTD__TYPES__GLOBAL_EPOCH_TPP__
#define __MSTD__TYPES__GLOBAL_EPOCH_TPP__

#include "global_epoch.hpp"

namespace mstd
{
    /**
     * @brief Get the current global epoch.
     *
     * @return The current value of the global epoch counter.
     */
    template <typename Domain>
    EpochValueType GlobalEpoch<Domain>::now() noexcept
    {
        return value.load(std::memory_order_relaxed);
    }

    /**
     * @brief Advance the global epoch by one.
     *
     * @return The new value of the global epoch counter after incrementing.
     */
    template <typename Domain>
    EpochValueType GlobalEpoch<Domain>::advance() noexcept
    {
        return value.fetch_add(1, std::memory_order_relaxed) + 1;
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__GLOBAL_EPOCH_TPP__
