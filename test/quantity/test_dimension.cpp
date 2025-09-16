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

using namespace mstd::units;
using namespace mstd::units::details;
using namespace mstd::pack;

// Small helper for cleaner static type asserts
template <class A, class B>
constexpr bool same = std::is_same_v<A, B>;

TEST_CASE("Enum indexing works for packs and dims")
{
    using d =
        dim<integer_pack<1, 2, 3, 4, 5, 6, 7>, integer_pack<8, 9, 10, 11>>;

    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Length> == 1);
    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Mass> == 2);
    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Time> == 3);
    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Current> == 4);
    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Temperature> == 5);
    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Amount> == 6);
    MSTD_STATIC_REQUIRE(d::si_exp<SIDimId::Luminous> == 7);

    MSTD_STATIC_REQUIRE(d::ex_exp<ExtraDimId::Angle> == 8);
    MSTD_STATIC_REQUIRE(d::ex_exp<ExtraDimId::Currency> == 9);
    MSTD_STATIC_REQUIRE(d::ex_exp<ExtraDimId::Info> == 10);
}

TEST_CASE("dim_mul_t basic cases")
{
    using L   = dim_length;     // L
    using T   = dim_time;       // T
    using A   = dim_angle;      // A
    using CUR = dim_currency;   // C

    using LT = dim_mul_t<L, T>;     // L T
    using LA = dim_mul_t<L, A>;     // L A
    using LC = dim_mul_t<L, CUR>;   // L C

    MSTD_STATIC_REQUIRE(
        same<
            LT,
            dim<integer_pack<1, 0, 1, 0, 0, 0, 0>, integer_pack<0, 0, 0, 0>>>
    );
    MSTD_STATIC_REQUIRE(
        same<
            LA,
            dim<integer_pack<1, 0, 0, 0, 0, 0, 0>, integer_pack<1, 0, 0, 0>>>
    );
    MSTD_STATIC_REQUIRE(
        same<
            LC,
            dim<integer_pack<1, 0, 0, 0, 0, 0, 0>, integer_pack<0, 1, 0, 0>>>
    );
}

TEST_CASE("dim_div_t basic cases")
{
    using L = dim_length;
    using T = dim_time;
    using A = dim_angle;

    using V   = dim_div_t<L, T>;   // L T^-1
    using inv = dim_div_t<dim_scalar, L>;

    MSTD_STATIC_REQUIRE(
        same<
            V,
            dim<integer_pack<1, 0, -1, 0, 0, 0, 0>, integer_pack<0, 0, 0, 0>>>
    );
    MSTD_STATIC_REQUIRE(
        same<
            inv,
            dim<integer_pack<-1, 0, 0, 0, 0, 0, 0>, integer_pack<0, 0, 0, 0>>>
    );

    using A_over_T  = dim_div_t<A, T>;          // A T^-1
    using back_to_A = dim_mul_t<A_over_T, T>;   // -> A
    MSTD_STATIC_REQUIRE(same<back_to_A, A>);
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
        same<
            L2,
            dim<integer_pack<2, 0, 0, 0, 0, 0, 0>, integer_pack<0, 0, 0, 0>>>
    );

    // Negative power -> reciprocal
    using invT = dim_pow_t<T, -1>;
    MSTD_STATIC_REQUIRE(
        same<
            invT,
            dim<integer_pack<0, 0, -1, 0, 0, 0, 0>, integer_pack<0, 0, 0, 0>>>
    );

    // Zero power -> scalar
    using any  = dim_mul_t<dim_mul_t<L, T>, dim_mul_t<A, C>>;
    using any0 = dim_pow_t<any, 0>;
    MSTD_STATIC_REQUIRE(same<any0, dim_scalar>);
}

TEST_CASE("Associativity and distributivity hold for packs")
{
    using L = dim_length;
    using M = dim_mass;
    using T = dim_time;

    using left  = dim_mul_t<dim_mul_t<L, M>, T>;
    using right = dim_mul_t<L, dim_mul_t<M, T>>;
    MSTD_STATIC_REQUIRE(same<left, right>);

    // (A/B)/C == A/(B*C)
    using A         = dim_angle;
    using left_div  = dim_div_t<dim_div_t<L, T>, A>;
    using right_div = dim_div_t<L, dim_mul_t<T, A>>;
    MSTD_STATIC_REQUIRE(same<left_div, right_div>);

    // pow distributes over mul: (LM)^2 == L^2 M^2
    using LM2  = dim_pow_t<dim_mul_t<L, M>, 2>;
    using L2M2 = dim_mul_t<dim_pow_t<L, 2>, dim_pow_t<M, 2>>;
    MSTD_STATIC_REQUIRE(same<LM2, L2M2>);

    // pow over div: (L/T)^-1 == T/L
    using V    = dim_div_t<L, T>;
    using invV = dim_pow_t<V, -1>;
    MSTD_STATIC_REQUIRE(same<invV, dim_div_t<T, L>>);
}

TEST_CASE("Dimensionless detection")
{
    MSTD_STATIC_REQUIRE(is_dimensionless_v<dim_scalar>);

    using L = dim_length;
    using T = dim_time;

    using dimLess1 = dim_div_t<L, L>;
    using dimLess2 = dim_mul_t<dim_div_t<L, T>, dim_div_t<T, L>>;

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

    using T_per_A = dim_div_t<T, A>;
    using back    = dim_mul_t<T_per_A, A>;

    MSTD_STATIC_REQUIRE(same<back, T>);
}

TEST_CASE("Currency exponent propagation")
{
    using C = dim_currency;
    using T = dim_time;

    using burn   = dim_div_t<C, T>;      // CUR T^-1
    using total1 = dim_mul_t<burn, T>;   // -> CUR
    MSTD_STATIC_REQUIRE(same<total1, C>);

    using C2   = dim_pow_t<C, 2>;
    using bps  = dim_div_t<C2, T>;    // CUR^2 T^-1
    using back = dim_div_t<bps, C>;   // -> CUR T^-1
    MSTD_STATIC_REQUIRE(same<back, burn>);
}