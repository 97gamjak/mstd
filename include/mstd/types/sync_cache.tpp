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

#include <mutex>

#include "mstd/exceptions/exceptions.hpp"
#include "sync_cache.hpp"

namespace mstd
{
    /**
     * @brief Construct a new Sync Cache object.
     *
     * @param compute The function to compute the value if it is not valid.
     */
    template <typename T>
    SyncCache<T>::SyncCache(const std::function<T()>& compute)
        : _compute(compute), _isValid(false)
    {
    }

    /**
     * @brief Get the cached value.
     * If the value is not valid and a compute function is set, it will compute
     * the value. (Thread-safe)
     */
    template <typename T>
    T SyncCache<T>::get()
    {
        {
            std::shared_lock lock(_mutex);
            if (_isValid)
            {
                return _value;
            }
        }

        std::unique_lock uniqueLock(_mutex);
        if (!_isValid && _compute)
        {
            _value   = (*_compute)();
            _isValid = true;
            return _value;
        }

        throw mstd::RuntimeError(
            "Cache value is not valid and no compute function is set."
        );
    }

    /**
     * @brief Invalidate the cached value.
     * (Thread-safe)
     */
    template <typename T>
    void SyncCache<T>::invalidate()
    {
        std::unique_lock lock(_mutex);
        _isValid = false;
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__SYNC_CACHE_TPP__
