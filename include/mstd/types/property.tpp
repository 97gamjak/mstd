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

#include "mstd/debugging.hpp"
#include "mstd/types/global_epoch.hpp"
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
    const T& Property<T>::get() const
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
     * @brief Constructs a CacheProperty with the given value.
     *
     * @param value The initial value for the CacheProperty.
     */
    template <typename T, typename Domain>
    CacheProperty<T, Domain>::CacheProperty(const T& value)
        : Property<T>(value), _version{GlobalEpoch<Domain>::advance()}
    {
    }

    /**
     * @brief Copy constructor for CacheProperty.
     */
    template <typename T, typename Domain>
    CacheProperty<T, Domain>::CacheProperty(const CacheProperty& other)
        : Property<T>(other.get()), _version{GlobalEpoch<Domain>::advance()}
    {
    }

    /**
     * @brief Copy assignment operator for CacheProperty.
     */
    template <typename T, typename Domain>
    CacheProperty<T, Domain>& CacheProperty<T, Domain>::operator=(
        const CacheProperty& other
    )
    {
        set(other.get());
        return *this;
    }

    /**
     * @brief Sets the value of the CacheProperty and invalidates the cache.
     */
    template <typename T, typename Domain>
    void CacheProperty<T, Domain>::set(const T& value)
    {
        MSTD_ASSERT_NOT_HOT("CacheProperty::set");
        _version = GlobalEpoch<Domain>::advance();
        Property<T>::set(value);
    }

    /**
     * @brief Retrieves the version of the CacheProperty.
     */
    template <typename T, typename Domain>
    const EpochValueType& CacheProperty<T, Domain>::version() const
    {
        return _version;
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__PROPERTY_TPP__
