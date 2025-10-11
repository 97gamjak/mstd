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

#ifndef __MSTD_UNITS_RATIO_HPP__
#define __MSTD_UNITS_RATIO_HPP__

#include <concepts>
#include <ratio>

namespace mstd
{

    namespace details
    {

        template <class R, int K>
        struct ratio_pow_impl;

        template <class R>
        struct ratio_pow_impl<R, 0>
        {
            using type = std::ratio<1>;
        };

        template <class R, int K>
        struct ratio_pow_impl
        {
            // exponentiation by squaring (works for K>0 and K<0)
            using base =
                std::conditional_t<(K < 0), std::ratio<R::den, R::num>, R>;
            static constexpr int e = (K < 0 ? -K : K);

            template <int N, class Acc, class Cur>
            struct step
            {
                using type = std::conditional_t<
                    (N == 0),
                    Acc,
                    typename step<
                        (N >> 1),
                        std::conditional_t<
                            (N & 1),
                            std::ratio_multiply<Acc, Cur>,
                            Acc>,
                        std::ratio_multiply<Cur, Cur>>::type>;
            };

            using type = typename step<e, std::ratio<1>, base>::type;
        };

    }   // namespace details

    template <class R, int K>
    using ratio_pow_t = typename details::ratio_pow_impl<R, K>::type;

    template <class T>
    static constexpr bool isStdRatio_v = requires {
        T::num;
        T::den;
    };

    template <class T>
    concept StdRatio = requires {
        { T::num } -> std::convertible_to<long long>;
        { T::den } -> std::convertible_to<long long>;
    };

    template <typename T>
    inline constexpr long double ratio_v = 1.0L;

    template <StdRatio T>
    static constexpr long double ratio_v<T> =
        static_cast<long double>(T::num) / static_cast<long double>(T::den);

}   // namespace mstd

#endif   // __MSTD_UNITS_RATIO_HPP__