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

#ifndef __MSTD_UNIT_OPERATIONS_HPP__
#define __MSTD_UNIT_OPERATIONS_HPP__

#include "dimension.hpp"
#include "mstd/math.hpp"
#include "mstd/pack.hpp"
#include "mstd/ratio.hpp"
#include "unit.hpp"

namespace mstd::units
{
    template <class U>
    struct unit_traits;

    template <class Dim, class Ratio, ratio::StdRatio GlobalRatio>
    struct unit_traits<unit<Dim, Ratio, GlobalRatio>>
    {
        using dim    = Dim;
        using ratio  = typename unit<Dim, Ratio, GlobalRatio>::ratio;
        using global = GlobalRatio;
        static constexpr long double factor  = 1.0L;
        static constexpr bool        is_real = false;
    };

    template <
        class Dim,
        long double F,
        class Ratio,
        ratio::StdRatio GlobalRatio>
    struct unit_traits<real_unit<Dim, F, Ratio, GlobalRatio>>
    {
        using dim    = Dim;
        using ratio  = typename real_unit<Dim, F, Ratio, GlobalRatio>::ratio;
        using global = GlobalRatio;
        static constexpr long double factor  = F;
        static constexpr bool        is_real = true;
    };

    template <class Unit1, class Unit2>
    inline constexpr bool same_dimension_v = std::is_same_v<
        typename unit_traits<Unit1>::dim,
        typename unit_traits<Unit2>::dim>;

    template <class Unit1, class Unit2>
    inline constexpr bool compatible_units_v = same_dimension_v<Unit1, Unit2>;

    template <
        class Dim,
        long double FactorToSI,
        class Ratio,
        ratio::StdRatio GlobalRatio,
        bool            AnyReal>
    struct build_unit_impl;

    template <
        class Dim,
        long double F,
        class Ratio,
        ratio::StdRatio GlobalRatio>
    struct build_unit_impl<Dim, F, Ratio, GlobalRatio, false>
    {
        using type = unit<Dim, Ratio, GlobalRatio>;
    };

    template <
        class Dim,
        long double F,
        class Ratio,
        ratio::StdRatio GlobalRatio>
    struct build_unit_impl<Dim, F, Ratio, GlobalRatio, true>
    {
        using type = real_unit<Dim, F, Ratio, GlobalRatio>;
    };

    template <
        class Dim,
        long double FactorToSI,
        class Ratio,
        ratio::StdRatio GlobalRatio,
        bool            AnyReal>
    using build_unit_t = typename build_unit_impl<
        Dim,
        FactorToSI,
        Ratio,
        GlobalRatio,
        (AnyReal || (FactorToSI != static_cast<long double>(1)))>::type;

    template <class Unit1, class Unit2>
    struct unit_mul_impl
    {
        using T1    = unit_traits<Unit1>;
        using T2    = unit_traits<Unit2>;
        using dim   = dim_mul_t<typename T1::dim, typename T2::dim>;
        using ratio = dim_ratio_mul_t<typename T1::ratio, typename T2::ratio>;
        using global =
            std::ratio_multiply<typename T1::global, typename T2::global>;

        static constexpr long double f        = T1::factor * T2::factor;
        static constexpr bool        any_real = T1::is_real || T2::is_real;
        using type = build_unit_t<dim, f, ratio, global, any_real>;
    };

    template <class Unit1, class Unit2>
    using unit_mul = typename unit_mul_impl<Unit1, Unit2>::type;

    template <class Unit1, class Unit2>
    struct unit_div_impl
    {
        using T1    = unit_traits<Unit1>;
        using T2    = unit_traits<Unit2>;
        using dim   = dim_div_t<typename T1::dim, typename T2::dim>;
        using ratio = dim_ratio_div_t<typename T1::ratio, typename T2::ratio>;
        using global =
            std::ratio_divide<typename T1::global, typename T2::global>;

        static constexpr long double factor   = T1::factor / T2::factor;
        static constexpr bool        any_real = T1::is_real || T2::is_real;
        using type = build_unit_t<dim, factor, ratio, global, any_real>;
    };
    template <class Unit1, class Unit2>
    using unit_div = typename unit_div_impl<Unit1, Unit2>::type;

    template <class Unit, int Exp>
    struct unit_pow_impl
    {
        using T      = unit_traits<Unit>;
        using dim    = dim_pow_t<typename T::dim, Exp>;
        using ratio  = dim_ratio_pow_t<typename T::ratio, Exp>;
        using global = ratio::template ratio_pow_t<typename T::global, Exp>;
        static constexpr long double factor   = math::power(T::factor, Exp);
        static constexpr bool        any_real = T::is_real;
        using type = build_unit_t<dim, factor, ratio, global, any_real>;
    };
    template <class Unit, int Exp>
    using unit_pow = typename unit_pow_impl<Unit, Exp>::type;

    template <class Unit>
    inline constexpr long double ratio_v =
        pack::ratio_pack_v<typename unit_traits<Unit>::ratio::si> *
        pack::ratio_pack_v<typename unit_traits<Unit>::ratio::ex> *
        ratio::ratio_v<typename unit_traits<Unit>::global>;

    template <class Unit>
    inline constexpr long double factor_v = unit_traits<Unit>::factor;

    template <class Unit>
    inline constexpr long double scale_v = factor_v<Unit> * ratio_v<Unit>;

}   // namespace mstd::units

#endif   // __MSTD_UNIT_OPERATIONS_HPP__