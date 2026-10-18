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

#ifndef __MSTD__SHARED__TRAITS__GETTER_HPP__
#define __MSTD__SHARED__TRAITS__GETTER_HPP__

#include <type_traits>

namespace mstd::detail
{
    /**
     * Determines if a type should be passed by value.
     */
    template <typename T>
    inline constexpr bool pass_by_value_v =
        std::is_trivially_copyable_v<T> && sizeof(T) <= 2 * sizeof(void *);

    /**
     * Returns the value of a member, either by value or by const reference,
     * depending on the type.
     */
    template <typename T>
    [[nodiscard]] constexpr decltype(auto) getter_value(const T &v) noexcept
    {
        if constexpr (pass_by_value_v<T>)
            return T{v};   // returns T (by value)
        else
            return (v);   // returns const T&
    }

    template <typename T>
    using getter_t =
        std::conditional_t<detail::pass_by_value_v<T>, T, const T &>;
}   // namespace mstd::detail

#endif   // __MSTD__SHARED__TRAITS__GETTER_HPP__
