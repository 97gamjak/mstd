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

#ifndef __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_IMPL_HPP__
#define __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_IMPL_HPP__

#include "mstd/math.hpp"

namespace mstd
{

    /**
     * @brief Generic helper returning the energy/force pair of a Lie potential.
     *
     * @tparam M attractive exponent.
     * @tparam N repulsive exponent.
     * @tparam Rep numeric representation.
     * @param c1 attractive prefactor.
     * @param c2 repulsive prefactor.
     * @param r  inter-particle distance.
     */
    template <size_t M, size_t N, typename Rep>
    static inline constexpr std::pair<Rep, Rep> liePotential(
        Rep c1,
        Rep c2,
        Rep r
    )
    {
        const auto rm = cpow<M>(r);
        const auto rn = cpow<N>(r);

        const auto c1rm   = c1 / rm;
        const auto c2rn   = c2 / rn;
        const auto energy = -c1rm + c2rn;
        const auto force  = M * c1rm / r - N * c2rn / r;

        return {energy, force};
    }

    /**
     * @brief Optimized specialization for the canonical 6-12 Lennard-Jones form.
     */
    template <size_t M, size_t N, typename Rep>
    requires(M == 6 && N == 12)
    static inline constexpr std::pair<Rep, Rep> liePotential(
        Rep c1,
        Rep c2,
        Rep r
    )
    {
        const auto r2     = r * r;
        const auto r6     = r2 * r2 * r2;
        const auto r12    = r6 * r6;
        const auto c1r6   = c1 / r6;
        const auto c2r12  = c2 / r12;
        const auto energy = -c1r6 + c2r12;
        const auto force  = 6 * c1r6 / r - 12 * c2r12 / r;

        return {energy, force};
    }

    /// @brief Convenience wrapper for Lennard-Jones (6-12) parameters.
    template <typename Rep>
    static constexpr std::pair<Rep, Rep> ljPotential(Rep c1, Rep c2, Rep r)
    {
        return liePotential<6, 12, Rep>(c1, c2, r);
    }

}   // namespace mstd

#endif   // __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_IMPL_HPP__
