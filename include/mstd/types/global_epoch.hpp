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

#ifndef __MSTD__TYPES__GLOBAL_EPOCH_HPP__
#define __MSTD__TYPES__GLOBAL_EPOCH_HPP__

#include <atomic>
#include <cstdint>

namespace mstd
{
    using EpochValueType = std::uint64_t;

    /**
     * @brief A global epoch counter for a specific domain.
     *
     * This class provides a static atomic counter that can be used to track
     * the global epoch for a given domain. It supports querying the current
     * epoch and advancing it.
     */
    template <typename Domain>
    struct GlobalEpoch
    {
        alignas(64) static inline std::atomic<EpochValueType> value{0};

        [[nodiscard]]
        static EpochValueType now() noexcept;

        [[nodiscard]]
        static EpochValueType advance() noexcept;
    };
}   // namespace mstd

#ifndef __MSTD__TYPES__GLOBAL_EPOCH_TPP__
#include "global_epoch.tpp"
#endif   // __MSTD__TYPES__GLOBAL_EPOCH_TPP__

#endif   // __MSTD__TYPES__GLOBAL_EPOCH_HPP__
