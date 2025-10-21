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

#ifndef __MSTD_RATIO_RATIO_HPP__
#define __MSTD_RATIO_RATIO_HPP__

#include <concepts>
#include <ratio>

#include "mstd/error.hpp"
#include "mstd/type_traits/ratio_traits.hpp"

namespace mstd
{

    /**
     * @brief Alias for std::ratio
     *
     * @tparam Num Numerator
     * @tparam Den Denominator (default: 1)
     */
    template <intmax_t Num, intmax_t Den = 1>
    using ratio = std::ratio<Num, Den>;

    namespace details
    {

        template <class R, int K>
        struct ratio_pow_impl
        {
            MSTD_COMPILE_FAIL("Unsupported ratio type");
        };

        template <RatioType R, int K>
        struct ratio_pow_impl<R, K>
        {
            // Handle negative exponents by flipping numerator/denominator
            using _invRatio = std::ratio<R::den, R::num>;
            using base      = std::conditional_t<(K < 0), _invRatio, R>;

            // Absolute value of the exponent
            static constexpr int e = (K < 0 ? -K : K);

            // Start from ratio<1,1>
            using type = std::conditional_t<
                e == 0,
                std::ratio<1>,
                typename std::ratio_multiply<
                    base,
                    typename ratio_pow_impl<base, e - 1>::type>::type>;
        };

        // --- Base cases ---
        template <RatioType R>
        struct ratio_pow_impl<R, 0>
        {
            using type = std::ratio<1>;
        };

        template <RatioType R>
        struct ratio_pow_impl<R, 1>
        {
            using type = R;
        };

    }   // namespace details

    template <class R, int K>
    using ratio_pow_t = typename details::ratio_pow_impl<R, K>::type;

}   // namespace mstd

#endif   // __MSTD_RATIO_RATIO_HPP__