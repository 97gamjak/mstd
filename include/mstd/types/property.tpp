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

#ifndef __MSTD__TYPES__PROPERTY_TPP__
#define __MSTD__TYPES__PROPERTY_TPP__

#include "property.hpp"

namespace mstd
{
    /**
     * @brief Constructs a Property with the given value.
     */
    template <typename T>
    Property<T>::Property(const T& value) : _value(value)
    {
    }

    /**
     * @brief Retrieves the value of the Property.
     */
    template <typename T>
    T Property<T>::get() const
    {
        return _value;
    }

    /**
     * @brief Sets the value of the Property.
     */
    template <typename T>
    void Property<T>::set(const T& value)
    {
        _value = value;
    }

    /**
     * @brief Constructs a CacheProperty with the given Cache value.
     */
    template <typename T>
    CacheProperty<T>::CacheProperty(const Cache<T>& cache)
        : Property<Cache<T>>(cache)
    {
    }

    /**
     * @brief Sets the value of the CacheProperty and invalidates the cache.
     */
    template <typename T>
    void CacheProperty<T>::set(const Cache<T>& cache)
    {
        cache.invalidate();
        Property<Cache<T>>::set(cache);
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__PROPERTY_TPP__
