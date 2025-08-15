#ifndef __MSTD_UNITS_QUANTITY_HPP__
#define __MSTD_UNITS_QUANTITY_HPP__

#include "dimension.hpp"
#include "ratio.hpp"

namespace mstd
{

    template <typename... Ratios>
    struct ratio_mul;

    template <>
    struct ratio_mul<>
    {
        using type = std::ratio<1>;
    };

    template <typename R1, typename... Ratios>
    struct ratio_mul<R1, Ratios...>
    {
        using type =
            std::ratio_multiply<R1, typename ratio_mul<Ratios...>::type>;
    };

    template <typename... Ratios>
    using ratio_mul_t = typename ratio_mul<Ratios...>::type;

    template <typename Dim, typename Pack>
    struct dim_scale;

    template <int L, int M, int T, int K, int N, int I, int Cd, typename Pack>
    struct dim_scale<dimension<L, M, T, K, N, I, Cd>, Pack>
    {
        using type = ratio_mul_t<
            ratio_pow_t<typename Pack::L::ratio, L>,
            ratio_pow_t<typename Pack::M::ratio, M>,
            ratio_pow_t<typename Pack::T::ratio, T>,
            ratio_pow_t<typename Pack::K::ratio, K>,
            ratio_pow_t<typename Pack::N::ratio, N>,
            ratio_pow_t<typename Pack::I::ratio, I>,
            ratio_pow_t<typename Pack::Cd::ratio, Cd> >;
    };

    template <typename Dim, typename Pack>
    using dim_scale_t = typename dim_scale<Dim, Pack>::type;

    template <typename T, typename Dim, typename Pack>
    class Quantity
    {
       public:
        using value_type = T;
        using dim_type   = Dim;
        using pack_type  = Pack;
        using eff_scale  = dim_scale_t<Dim, Pack>;

        constexpr Quantity() = default;

        constexpr explicit Quantity(const T value);

        template <typename OtherPack>
        constexpr Quantity(const Quantity<T, Dim, OtherPack>& other);

        constexpr T value() const;
        constexpr T si() const { return _si; }

        template <typename OtherPack>
        constexpr Quantity operator+(
            const Quantity<T, Dim, OtherPack>& other
        ) const;

        template <typename OtherPack>
        constexpr Quantity operator-(
            const Quantity<T, Dim, OtherPack>& other
        ) const;

        static constexpr Quantity from_si(const T si);

       private:
        T _si;
    };

    template <typename T, typename Dim, typename Pack>
    constexpr Quantity<T, Dim, Pack>::Quantity(const T value)
        : _si(value * static_cast<T>(eff_scale::num) /
              static_cast<T>(eff_scale::den))
    {
    }

    template <typename T, typename Dim, typename Scale>
    template <typename OtherPack>
    constexpr Quantity<T, Dim, Scale>::Quantity(
        const Quantity<T, Dim, OtherPack>& other
    )
        : _si(other._si)
    {
    }

    template <typename T, typename Dim, typename Pack>
    constexpr T Quantity<T, Dim, Pack>::value() const
    {
        return _si * static_cast<T>(eff_scale::den) /
               static_cast<T>(eff_scale::num);
    }

    template <typename T, typename Dim, typename Pack>
    template <typename OtherPack>
    constexpr Quantity<T, Dim, Pack> Quantity<T, Dim, Pack>::operator+(
        const Quantity<T, Dim, OtherPack>& other
    ) const
    {
        return from_si(_si + other._si);
    }

    template <typename T, typename Dim, typename Pack>
    template <typename OtherPack>
    constexpr Quantity<T, Dim, Pack> Quantity<T, Dim, Pack>::operator-(
        const Quantity<T, Dim, OtherPack>& other
    ) const
    {
        return from_si(_si - other._si);
    }

    template <typename T, typename Dim, typename Pack>
    constexpr Quantity<T, Dim, Pack> from_si(const T si)
    {
        Quantity<T, Dim, Pack> q;
        q._si = si;
        return q;
    }

    template <
        typename T,
        typename Dim1,
        typename Pack1,
        typename Dim2,
        typename Pack2>
    constexpr auto operator*(
        const Quantity<T, Dim1, Pack1>& lhs,
        const Quantity<T, Dim2, Pack2>& rhs
    )
    {
        using D = Dimension_add<Dim1, Dim2>;
        return Quantity<T, Dim1, Pack1>::from_si(lhs.si() * rhs.si());
    }

    template <
        typename T,
        typename Dim1,
        typename Pack1,
        typename Dim2,
        typename Pack2>
    constexpr auto operator/(
        const Quantity<T, Dim1, Pack1>& lhs,
        const Quantity<T, Dim2, Pack2>& rhs
    )
    {
        using D = Dimension_sub<Dim1, Dim2>;
        return Quantity<T, Dim1, Pack1>::from_si(lhs.si() / rhs.si());
    }

}   // namespace mstd

#endif   // __MSTD_UNITS_QUANTITY_HPP__