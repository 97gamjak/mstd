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

#ifndef __MSTD__TYPES__CACHE__CACHE_DETAIL_TPP__
#define __MSTD__TYPES__CACHE__CACHE_DETAIL_TPP__

#include "cache_detail.hpp"
#include "mstd/types/global_epoch.hpp"

namespace mstd::detail
{
    /**
     * @brief Get the current epoch of every domain in the list.
     *
     * @return The current epochs of all domains in the list.
     */
    template <typename... Ds>
    typename DomainList<Ds...>::Epochs DomainList<Ds...>::now() noexcept
    {
        return {GlobalEpoch<Ds>::now()...};
    }

    /**
     * @brief Check if the epochs are current for every domain in the list.
     *
     * @param epochs The epochs to check.
     * @return True if no domain in the list advanced since `epochs`.
     */
    template <typename... Ds>
    bool DomainList<Ds...>::isCurrent(const Epochs& epochs) noexcept
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            return ((epochs[I] == GlobalEpoch<Ds>::now()) && ...);
        }(std::index_sequence_for<Ds...>{});
    }

    /**
     * @brief Check if the atomic epochs are current for every domain in the
     * list.
     *
     * @param epochs The atomic epochs to check.
     * @return True if no domain in the list advanced since `epochs`.
     */
    template <typename... Ds>
    bool DomainList<Ds...>::isCurrent(const AtomicEpochs& epochs) noexcept
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            return (
                (epochs[I].load(std::memory_order_acquire) ==
                 GlobalEpoch<Ds>::now()) &&
                ...
            );
        }(std::index_sequence_for<Ds...>{});
    }

    /**
     * @brief Publish the current epoch for every domain in the list.
     *
     * @param epochs The atomic epochs to update.
     */
    template <typename... Ds>
    void DomainList<Ds...>::publish(AtomicEpochs& epochs) noexcept
    {
        [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            (epochs[I].store(GlobalEpoch<Ds>::now(), std::memory_order_release),
             ...);
        }(std::index_sequence_for<Ds...>{});
    }
}   // namespace mstd::detail

#endif   // __MSTD__TYPES__CACHE__CACHE_DETAIL_TPP__
