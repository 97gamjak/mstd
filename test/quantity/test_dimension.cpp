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

#include <type_traits>

#include "mstd/pack.hpp"
#include "mstd/quantity.hpp"
#include "test_utils.hpp"

using namespace mstd;
using namespace mstd::details;

TEST_CASE("Enum indexing works for packs and dims")
{
    using siRatio = RationalPack<
        Rational<1>,
        Rational<2>,
        Rational<3>,
        Rational<4>,
        Rational<5>,
        Rational<6>,
        Rational<7>>;

    using exRatio =
        RationalPack<Rational<8>, Rational<9>, Rational<10>, Rational<11>>;

    using d = Dim<siRatio, exRatio>;

    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Length> == 1);
    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Mass> == 2);
    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Time> == 3);
    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Current> == 4);
    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Temperature> == 5);
    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Amount> == 6);
    MSTD_STATIC_REQUIRE(d::si_exp_v<SIDimId::Luminous> == 7);

    MSTD_STATIC_REQUIRE(d::ex_exp_v<ExtraDimId::Angle> == 8);
    MSTD_STATIC_REQUIRE(d::ex_exp_v<ExtraDimId::Currency> == 9);
    MSTD_STATIC_REQUIRE(d::ex_exp_v<ExtraDimId::Info> == 10);
}

TEST_CASE("mul_type_t basic cases")
{
    using L   = dim_length;     // L
    using T   = dim_time;       // T
    using A   = dim_angle;      // A
    using CUR = dim_currency;   // C

    using LT = mul_type_t<L, T>;     // L T
    using LA = mul_type_t<L, A>;     // L A
    using LC = mul_type_t<L, CUR>;   // L C

    MSTD_STATIC_REQUIRE(
        is_same_v<
            LT,
            Dim<IntegerPack<1, 0, 1, 0, 0, 0, 0>, IntegerPack<0, 0, 0, 0>>>
    );
    MSTD_STATIC_REQUIRE(
        is_same_v<
            LA,
            Dim<IntegerPack<1, 0, 0, 0, 0, 0, 0>, IntegerPack<1, 0, 0, 0>>>
    );
    MSTD_STATIC_REQUIRE(
        is_same_v<
            LC,
            Dim<IntegerPack<1, 0, 0, 0, 0, 0, 0>, IntegerPack<0, 1, 0, 0>>>
    );
}

TEST_CASE("div_type_t basic cases")
{
    using L = dim_length;
    using T = dim_time;
    using A = dim_angle;

    using V   = div_type_t<L, T>;   // L T^-1
    using inv = div_type_t<dim_scalar, L>;

    MSTD_STATIC_REQUIRE(
        is_same_v<
            V,
            Dim<IntegerPack<1, 0, -1, 0, 0, 0, 0>, IntegerPack<0, 0, 0, 0>>>
    );
    MSTD_STATIC_REQUIRE(
        is_same_v<
            inv,
            Dim<IntegerPack<-1, 0, 0, 0, 0, 0, 0>, IntegerPack<0, 0, 0, 0>>>
    );

    using A_over_T  = div_type_t<A, T>;          // A T^-1
    using back_to_A = mul_type_t<A_over_T, T>;   // -> A
    MSTD_STATIC_REQUIRE(is_same_v<back_to_A, A>);
}

TEST_CASE("dim_pow_t powers and identities")
{
    using L = dim_length;
    using T = dim_time;
    using A = dim_angle;
    using C = dim_currency;

    // Square length -> area
    using L2 = dim_pow_t<L, 2>;
    MSTD_STATIC_REQUIRE(
        is_same_v<
            L2,
            Dim<IntegerPack<2, 0, 0, 0, 0, 0, 0>, IntegerPack<0, 0, 0, 0>>>
    );

    // Negative power -> reciprocal
    using invT = dim_pow_t<T, -1>;
    MSTD_STATIC_REQUIRE(
        is_same_v<
            invT,
            Dim<IntegerPack<0, 0, -1, 0, 0, 0, 0>, IntegerPack<0, 0, 0, 0>>>
    );

    // Zero power -> scalar
    using any  = mul_type_t<mul_type_t<L, T>, mul_type_t<A, C>>;
    using any0 = dim_pow_t<any, 0>;
    MSTD_STATIC_REQUIRE(is_same_v<any0, dim_scalar>);
}

TEST_CASE("Associativity and distributivity hold for packs")
{
    using L = dim_length;
    using M = dim_mass;
    using T = dim_time;

    using left  = mul_type_t<mul_type_t<L, M>, T>;
    using right = mul_type_t<L, mul_type_t<M, T>>;
    MSTD_STATIC_REQUIRE(is_same_v<left, right>);

    // (A/B)/C == A/(B*C)
    using A         = dim_angle;
    using left_div  = div_type_t<div_type_t<L, T>, A>;
    using right_div = div_type_t<L, mul_type_t<T, A>>;
    MSTD_STATIC_REQUIRE(is_same_v<left_div, right_div>);

    // pow distributes over mul: (LM)^2 == L^2 M^2
    using LM2  = dim_pow_t<mul_type_t<L, M>, 2>;
    using L2M2 = mul_type_t<dim_pow_t<L, 2>, dim_pow_t<M, 2>>;
    MSTD_STATIC_REQUIRE(is_same_v<LM2, L2M2>);

    // pow over div: (L/T)^-1 == T/L
    using V    = div_type_t<L, T>;
    using invV = dim_pow_t<V, -1>;
    MSTD_STATIC_REQUIRE(is_same_v<invV, div_type_t<T, L>>);
}

TEST_CASE("Dimensionless detection")
{
    MSTD_STATIC_REQUIRE(is_dimensionless_v<dim_scalar>);

    using L = dim_length;
    using T = dim_time;

    using dimLess1 = div_type_t<L, L>;
    using dimLess2 = mul_type_t<div_type_t<L, T>, div_type_t<T, L>>;

    MSTD_STATIC_REQUIRE(is_dimensionless_v<dimLess1>);
    MSTD_STATIC_REQUIRE(is_dimensionless_v<dimLess2>);

    using A = dim_angle;
    MSTD_STATIC_REQUIRE(!is_dimensionless_v<A>);
}

TEST_CASE("Mixed SI + Extra: cancellation to SI")
{
    // (Time / Angle) * Angle -> Time  (tests cross-pack cancellation)
    using T = dim_time;
    using A = dim_angle;

    using T_per_A = div_type_t<T, A>;
    using back    = mul_type_t<T_per_A, A>;

    MSTD_STATIC_REQUIRE(is_same_v<back, T>);
}

TEST_CASE("Currency exponent propagation")
{
    using C = dim_currency;
    using T = dim_time;

    using burn   = div_type_t<C, T>;      // CUR T^-1
    using total1 = mul_type_t<burn, T>;   // -> CUR
    MSTD_STATIC_REQUIRE(is_same_v<total1, C>);

    using C2   = dim_pow_t<C, 2>;
    using bps  = div_type_t<C2, T>;    // CUR^2 T^-1
    using back = div_type_t<bps, C>;   // -> CUR T^-1
    MSTD_STATIC_REQUIRE(is_same_v<back, burn>);
}