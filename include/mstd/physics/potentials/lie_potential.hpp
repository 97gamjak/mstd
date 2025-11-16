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

#include "mstd/math.hpp"

namespace mstd
{
    template <size_t M, size_t N, typename Rep = double>
    class LiePotential
    {
       private:
        static constexpr size_t _M = M;
        static constexpr size_t _N = N;

        Rep _coeff1{};
        Rep _coeff2{};

       public:
        constexpr LiePotential(Rep c1, Rep c2) : _coeff1(c1), _coeff2(c2) {}

        virtual Rep evalEnergy(const Rep r)
        {
            return -_coeff1 / cpow<_M>(r) + _coeff2 / cpow<_N>(r);
        }

        virtual Rep evalEnergy(const Rep r)
        requires(_M == 6 && _N == 12)
        {
            const auto r2        = r * r;
            const auto r6        = r2 * r2 * r2;
            const auto r12       = r6 * r6;
            const auto coeff1r6  = _coeff1 / r6;
            const auto coeff2r12 = _coeff2 / r12;
            return -coeff1r6 + coeff2r12;
        }

        virtual Rep evalForce(const Rep r)
        {
            return _M * _coeff1 / r / cpow<_M>(r) -
                   _N * _coeff2 / r / cpow<_N>(r);
        }

        virtual Rep evalForce(const Rep r)
        requires(_M == 6 && _N == 12)
        {
            const auto r2        = r * r;
            const auto r6        = r2 * r2 * r2;
            const auto r12       = r6 * r6;
            const auto coeff1r6  = _coeff1 / r6;
            const auto coeff2r12 = _coeff2 / r12;
            return 6 * coeff1r6 / r - 12 * coeff2r12 / r;
        }

        virtual std::pair<Rep, Rep> eval(const Rep r)
        {
            const auto rm = cpow<_M>(r);
            const auto rn = cpow<_N>(r);

            const auto coeff1rm = _coeff1 / rm;
            const auto coeff2rn = _coeff2 / rn;
            const auto energy   = -coeff1rm + coeff2rn;
            const auto force    = _M * coeff1rm / r - _N * coeff2rn / r;

            return {energy, force};
        }

        virtual std::pair<Rep, Rep> eval(const Rep r)
        requires(_M == 6 && _N == 12)
        {
            const auto r2        = r * r;
            const auto r6        = r2 * r2 * r2;
            const auto r12       = r6 * r6;
            const auto coeff1r6  = _coeff1 / r6;
            const auto coeff2r12 = _coeff2 / r12;
            const auto energy    = -coeff1r6 + coeff2r12;
            const auto force     = 6 * coeff1r6 / r - 12 * coeff2r12 / r;

            return {energy, force};
        }
    };

    template <typename Rep = double>
    using LJPotential = LiePotential<6, 12, Rep>;

    template <size_t M, size_t N, typename Rep = double>
    class LieShiftedPotential : public LiePotential<M, N, Rep>
    {
       private:
        using _Base = LiePotential<M, N, Rep>;

        Rep _radialCutoff{};
        Rep _energyCutoff{};
        Rep _forceCutoff{};

       public:
        constexpr LieShiftedPotential(Rep c1, Rep c2, Rep rc)
            : LiePotential<N, M, Rep>(c1, c2), _radialCutoff(rc)
        {
            _energyCutoff, _forceCutoff = this->eval(_radialCutoff);
        }

        Rep evalEnergy(const Rep r) override
        {
            return _Base::evalEnergy(r) - _energyCutoff +
                   _forceCutoff(r - _radialCutoff);
        }

        Rep evalForce(const Rep r)
        {
            return _Base::evalForce(r) - _forceCutoff;
        }

        std::pair<Rep, Rep> eval(const Rep r) override
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