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

#ifndef __MSTD__TYPES__PROPERTY_HPP__
#define __MSTD__TYPES__PROPERTY_HPP__

#include <cstddef>

namespace mstd
{
    /**
     * @brief A simple property class that encapsulates a value of type T.
     */
    template <typename T>
    class Property
    {
       private:
        T _value;

       public:
        Property() = default;
        explicit Property(const T& value);

        T            get() const;
        virtual void set(const T& value);
    };

    /**
     * @brief A property class that tracks the version of the value and allows
     * cache invalidation.
     */
    template <typename T>
    class CacheProperty : public Property<T>
    {
       private:
        size_t _version = 0;

       public:
        using Property<T>::Property;

        [[nodiscard]] size_t version() const noexcept;

        void set(const T& value) override;
    };
}   // namespace mstd

#ifndef __MSTD__TYPES__PROPERTY_TPP__
#include "property.tpp"
#endif

#endif   // __MSTD__TYPES__PROPERTY_HPP__
