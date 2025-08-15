#ifndef __MSTD_UNITS_DIMENSION_HPP__
#define __MSTD_UNITS_DIMENSION_HPP__

#include <array>
#include <cstddef>

namespace mstd
{

    template <int L, int M, int T, int Th, int N, int I, int Cd>
    class dimension
    {
       public:
        static constexpr std::size_t           rank = 7;
        static constexpr std::array<int, rank> exponents{L, M, T, Th, N, I, Cd};
    };

    template <typename D1, typename D2, std::size_t... Is>
    constexpr auto dimension_add_impl(std::index_sequence<Is...>)
    {
        return dimension<D1::exponents[Is] + D2::exponents[Is]...>{};
    }

    template <typename D1, typename D2, std::size_t... Is>
    constexpr auto dimension_sub_impl(std::index_sequence<Is...>)
    {
        return dimension<D1::exponents[Is] - D2::exponents[Is]...>{};
    }

    template <typename D1, typename D2>
    using Dimension_add = decltype(dimension_add_impl<D1, D2>(
        std::make_index_sequence<D1::rank>{}
    ));

    template <typename D1, typename D2>
    using Dimension_sub = decltype(dimension_sub_impl<D1, D2>(
        std::make_index_sequence<D1::rank>{}
    ));

}   // namespace mstd

namespace mstd
{
    using D_Length   = dimension<1, 0, 0, 0, 0, 0, 0>;
    using D_Mass     = dimension<0, 1, 0, 0, 0, 0, 0>;
    using D_Time     = dimension<0, 0, 1, 0, 0, 0, 0>;
    using D_Temp     = dimension<0, 0, 0, 1, 0, 0, 0>;
    using D_Amount   = dimension<0, 0, 0, 0, 1, 0, 0>;
    using D_I        = dimension<0, 0, 0, 0, 0, 1, 0>;
    using D_Cd       = dimension<0, 0, 0, 0, 0, 0, 1>;
    using D_UnitLess = dimension<0, 0, 0, 0, 0, 0, 0>;

    using D_Velocity     = Dimension_sub<D_Length, D_Time>;
    using D_Acceleration = Dimension_sub<D_Velocity, D_Time>;

}   // namespace mstd

#endif   // __MSTD_UNITS_DIMENSION_HPP__