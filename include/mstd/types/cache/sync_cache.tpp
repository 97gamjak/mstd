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

#ifndef __MSTD__TYPES__SYNC_CACHE_TPP__
#define __MSTD__TYPES__SYNC_CACHE_TPP__

#include <cstddef>
#include <mutex>

#include "mstd/exceptions.hpp"
#include "sync_cache.hpp"

namespace mstd
{
    template <typename T, typename Owner, auto... Members>
    SyncCache<T, Owner, Members...>::SyncCache(Func compute) : _compute(compute)
    {
    }

    template <typename T, typename Owner, auto... Members>
    SyncCache<T, Owner, Members...>::SyncCache(const SyncCache& other)
        : _compute(other._compute)
    {
        const std::lock_guard lock(other._mutex);
        _value = other._value;
        _copyEpochs(other);
        _filled.store(other._filled.load(std::memory_order_relaxed));
    }

    template <typename T, typename Owner, auto... Members>
    SyncCache<T, Owner, Members...>& SyncCache<T, Owner, Members...>::operator=(
        const SyncCache& other
    )
    {
        if (this == &other)
            return *this;

        // never hold both locks at once
        std::optional<T> value;
        bool             filled = false;
        {
            const std::lock_guard lock(other._mutex);
            value  = other._value;
            filled = other._filled.load(std::memory_order_relaxed);
        }

        const std::lock_guard lock(_mutex);
        _filled.store(false);   // readers go to the slow path meanwhile
        _value   = std::move(value);
        _compute = other._compute;
        _copyEpochs(other);
        _filled.store(filled);
        return *this;
    }

    template <typename T, typename Owner, auto... Members>
    void SyncCache<T, Owner, Members...>::_copyEpochs(const SyncCache& other)
    {
        for (std::size_t i = 0; i < _epochs.size(); ++i)
        {
            _epochs[i].store(
                other._epochs[i].load(std::memory_order_relaxed),
                std::memory_order_relaxed
            );
        }
    }

    template <typename T, typename Owner, auto... Members>
    T SyncCache<T, Owner, Members...>::get(const Owner& owner) const
    {
        // lock-free hit: only reads
        if (_filled.load(std::memory_order_acquire) &&
            domains::isCurrent(_epochs))
            return *_value;

        const std::lock_guard lock(_mutex);

        // another thread may have recomputed while we waited for the lock
        if (_filled.load(std::memory_order_relaxed) &&
            domains::isCurrent(_epochs))
            return *_value;

        if (!_compute)
        {
            throw mstd::RuntimeError(
                "Cache value is not valid and no compute function is set."
            );
        }

        _value = _compute(detail::readSource(owner, owner.*Members)...);
        domains::publish(_epochs);
        _filled.store(true, std::memory_order_release);
        return *_value;
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__SYNC_CACHE_TPP__
