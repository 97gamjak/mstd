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

#ifndef __MSTD__TYPE_TRAITS__ENUM_TRAITS_HPP__
#define __MSTD__TYPE_TRAITS__ENUM_TRAITS_HPP__

#include <type_traits>

namespace mstd
{
    /**
     * @brief A helper type alias to extract the enum metadata type
     *
     * @tparam E The enum type
     */
    template <class E>
    using enum_meta_t = decltype(enum_meta(std::declval<E>()));

    /**
     * @brief A concept to check if a type has enum metadata
     *
     * This concept checks if a type is an enum and has a corresponding
     * enum_meta specialization, which is true for enums defined with MSTD_ENUM.
     *
     * @tparam E The type to check
     */
    template <typename E>
    concept has_enum_meta =
        std::is_enum_v<E> && requires { typename enum_meta_t<E>; };

}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__ENUM_TRAITS_HPP__