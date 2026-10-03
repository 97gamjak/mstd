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

#ifndef __MSTD__TYPES__REF_TPP__
#define __MSTD__TYPES__REF_TPP__

#include <memory>

#include "ref.hpp"

namespace mstd
{
    /**
     * @brief Construct a Ref from an lvalue reference.
     *
     * @tparam T referenced type
     * @param ref the object to refer to, must outlive this Ref
     */
    template <typename T>
    constexpr Ref<T>::Ref(T &ref) noexcept : _ref(ref)
    {
    }

    /**
     * @brief Converting constructor, e.g. Ref<Derived> -> Ref<Base> or
     * Ref<X> -> Ref<const X>.
     *
     * @tparam T referenced type
     * @tparam U source referenced type
     * @param other the Ref to convert from
     */
    template <typename T>
    template <typename U>
    requires(!std::same_as<U, T> && std::is_convertible_v<U &, T &>)
    constexpr Ref<T>::Ref(const Ref<U> &other) noexcept : _ref(other.get())
    {
    }

    /**
     * @brief Access the referenced object.
     *
     * @tparam T referenced type
     * @return T& the referenced object
     */
    template <typename T>
    constexpr T &Ref<T>::get() const noexcept
    {
        return _ref.get();
    }

    /**
     * @brief Dereference to the referenced object.
     *
     * @tparam T referenced type
     * @return T& the referenced object
     */
    template <typename T>
    constexpr T &Ref<T>::operator*() const noexcept
    {
        return get();
    }

    /**
     * @brief Member access on the referenced object.
     *
     * @tparam T referenced type
     * @return T* address of the referenced object
     */
    template <typename T>
    constexpr T *Ref<T>::operator->() const noexcept
    {
        return std::addressof(get());
    }

    /**
     * @brief Implicit conversion to the referenced type, so a Ref can be
     * passed to functions taking T&.
     *
     * @tparam T referenced type
     */
    template <typename T>
    constexpr Ref<T>::operator T &() const noexcept
    {
        return get();
    }

}   // namespace mstd

#endif   // __MSTD__TYPES__REF_TPP__
