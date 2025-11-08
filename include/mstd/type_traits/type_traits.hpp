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

#ifndef __TYPE_TRAITS__TYPE_TRAITS_HPP__
#define __TYPE_TRAITS__TYPE_TRAITS_HPP__

#include <type_traits>

namespace mstd
{
    template <auto T>
    requires std::is_integral_v<decltype(T)>
    using IntegralConst = std::integral_constant<decltype(T), T>;

    template <typename T>
    concept IntegralConstantType = requires {
        typename T::value_type;
        T::value;
    } && std::is_integral_v<typename T::value_type>;

    template <typename T, typename U>
    struct convert
    {
        MSTD_COMPILE_FAIL(
            "conversion between the given types is not supported"
        );
    };

    template <typename T, typename U>
    using convert_t = typename convert<T, U>::type;

}   // namespace mstd

#endif   // __TYPE_TRAITS__TYPE_TRAITS_HPP__