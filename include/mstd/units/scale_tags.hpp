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

#ifndef __MSTD_UNITS_SCALE_TAGS_HPP__
#define __MSTD_UNITS_SCALE_TAGS_HPP__

#include <ratio>

namespace mstd
{
    template <typename R>
    struct L
    {
        using ratio = R;
        using kind  = void;
    };
    template <typename R>
    struct M
    {
        using ratio = R;
        using kind  = void;
    };
    template <typename R>
    struct T
    {
        using ratio = R;
        using kind  = void;
    };
    template <typename R>
    struct Temp
    {
        using ratio = R;
        using kind  = void;
    };
    template <typename R>
    struct N
    {
        using ratio = R;
        using kind  = void;
    };
    template <typename R>
    struct I
    {
        using ratio = R;
        using kind  = void;
    };
    template <typename R>
    struct Candela
    {
        using ratio = R;
        using kind  = void;
    };

    using Ang = L<std::ratio<1, 10>>;
    using nm  = L<std::nano>;
    using um  = L<std::micro>;
    using mm  = L<std::milli>;
    using cm  = L<std::centi>;
    using dm  = L<std::deci>;
    using m   = L<std::ratio<1>>;
    using km  = L<std::kilo>;

    using ng = M<std::pico>;
    using ug = M<std::nano>;
    using mg = M<std::micro>;
    using g  = M<std::milli>;
    using kg = M<std::ratio<1>>;
    using t  = M<std::kilo>;
    using Mt = M<std::mega>;

    using fs  = T<std::femto>;
    using ps  = T<std::pico>;
    using ns  = T<std::nano>;
    using us  = T<std::micro>;
    using ms  = T<std::milli>;
    using s   = T<std::ratio<1>>;
    using min = T<std::ratio<60>>;
    using h   = T<std::ratio<3600>>;
    using d   = T<std::ratio<86400>>;
    using yr  = T<std::ratio<31536000>>;

    using K = Temp<std::ratio<1>>;

    using mol = N<std::ratio<1>>;

    using A = I<std::ratio<1>>;

    using Cd = Candela<std::ratio<1>>;

    // clang-format off
    template<typename X> struct is_L : std::false_type {};
    template<typename R> struct is_L<L<R>> : std::true_type {};

    template<typename X> struct is_T : std::false_type {};
    template<typename R> struct is_T<T<R>> : std::true_type {};

    template<typename X> struct is_M : std::false_type {};
    template<typename R> struct is_M<M<R>> : std::true_type {};

    template<typename X> struct is_K : std::false_type {};
    template<typename R> struct is_K<Temp<R>> : std::true_type {};

    template<typename X> struct is_N : std::false_type {};
    template<typename R> struct is_N<N<R>> : std::true_type {};

    template<typename X> struct is_I : std::false_type {};
    template<typename R> struct is_I<I<R>> : std::true_type {};

    template<typename X> struct is_Cd : std::false_type {};
    template<typename R> struct is_Cd<Candela<R>> : std::true_type {};

    template<typename X> inline constexpr bool is_L_v = is_L<X>::value;
    template<typename X> inline constexpr bool is_T_v = is_T<X>::value;
    template<typename X> inline constexpr bool is_M_v = is_M<X>::value;
    template<typename X> inline constexpr bool is_K_v = is_K<X>::value;
    template<typename X> inline constexpr bool is_N_v = is_N<X>::value;
    template<typename X> inline constexpr bool is_I_v = is_I<X>::value;
    template<typename X> inline constexpr bool is_Cd_v = is_Cd<X>::value;
    // clang-format on

    // clang-format off
    template <typename X> concept LengthTag = is_L<X>::value;
    template <typename X> concept TimeTag = is_T<X>::value;
    template <typename X> concept MassTag = is_M<X>::value;
    template <typename X> concept TemperatureTag = is_K<X>::value;
    template <typename X> concept AmountOfSubstanceTag = is_N<X>::value;
    template <typename X> concept CurrentTag = is_I<X>::value;
    template <typename X> concept LuminousIntensityTag = is_Cd<X>::value;
    // clang-format on

}   // namespace mstd

#endif   // __MSTD_UNITS_SCALE_TAGS_HPP__