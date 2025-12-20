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

#ifndef __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_HPP__
#define __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_HPP__

#include "lie_potential_impl.hpp"
#include "mstd/math.hpp"

namespace mstd
{
    /**
     * @brief Generalized Lennard-Jones-like pair potential.
     *
     * The template parameters define the attractive (\f$M\f$) and repulsive
     * (\f$N\f$) exponents of the potential. The representation type @p Rep is
     * typically `double`, but any floating-point type is acceptable.
     */
    template <size_t M, size_t N, typename Rep = double>
    class LiePotential
    {
       private:
        static constexpr size_t _M = M;
        static constexpr size_t _N = N;

        Rep _coeff1{};
        Rep _coeff2{};

       public:
        virtual ~LiePotential() = default;

        /**
         * @brief Constructs the potential with prefactors for the attractive
         *        and repulsive terms.
         *
         * @note The coefficients correspond to the terms
         *       \f$-c_1 r^{-M} + c_2 r^{-N}\f$ in the potential expression.
         *
         * @param c1 Coefficient for the attractive term.
         * @param c2 Coefficient for the repulsive term.
         */
        constexpr LiePotential(Rep c1, Rep c2) : _coeff1(c1), _coeff2(c2) {}

        /// @brief Evaluates only the potential energy at a distance @p r.
        virtual Rep evalEnergy(const Rep r) const
        {
            return -_coeff1 / cpow<_M>(r) + _coeff2 / cpow<_N>(r);
        }

        /// @brief Evaluates only the force magnitude at a distance @p r.
        virtual Rep evalForce(const Rep r) const
        {
            return _M * _coeff1 / r / cpow<_M>(r) -
                   _N * _coeff2 / r / cpow<_N>(r);
        }

        /// @brief Returns both energy and force evaluated at @p r.
        virtual std::pair<Rep, Rep> eval(const Rep r) const
        {
            return liePotential<M, N, Rep>(_coeff1, _coeff2, r);
        }
    };

    template <typename Rep = double>
    using LJPotential = LiePotential<6, 12, Rep>;

    /**
     * @brief Shifted variant that enforces zero energy/force at a cutoff.
     *
     * The constructor precomputes the energy and force at the radial cutoff so
     * that subsequent evaluations simply subtract the shift, yielding a smooth
     * truncation suitable for molecular dynamics simulations.
     */
    template <size_t M, size_t N, typename Rep = double>
    class LieShiftedPotential : public LiePotential<M, N, Rep>
    {
       private:
        using _Base = LiePotential<M, N, Rep>;

        Rep _radialCutoff{};
        Rep _energyCutoff{};
        Rep _forceCutoff{};

       public:
        /// @brief Builds the shifted potential from coefficients and cutoff
        ///        radius.
        constexpr LieShiftedPotential(Rep c1, Rep c2, Rep rc)
            : LiePotential<M, N, Rep>(c1, c2), _radialCutoff(rc)
        {
            std::tie(_energyCutoff, _forceCutoff) = _Base::eval(rc);
        }

        /// @brief Energy corrected so that it vanishes at the cutoff.
        Rep evalEnergy(const Rep r) const override
        {
            return _Base::evalEnergy(r) - _energyCutoff +
                   _forceCutoff * (r - _radialCutoff);
        }

        /// @brief Force corrected so that it vanishes at the cutoff.
        Rep evalForce(const Rep r) const override
        {
            return _Base::evalForce(r) - _forceCutoff;
        }

        /// @brief Returns the shifted energy/force pair evaluated at @p r.
        std::pair<Rep, Rep> eval(const Rep r) const override
        {
            const auto energy = evalEnergy(r);
            const auto force  = evalForce(r);
            return {energy, force};
        }
    };

    template <typename Rep = double>
    using LJShiftedPotential = LieShiftedPotential<6, 12, Rep>;

}   // namespace mstd

#endif   // __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_HPP__
