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

namespace mstd
{
    template <size_t N, size_t M, typename Rep = double>
    class LiePotential
    {
       private:
        static constexpr size_t _N = N;
        static constexpr size_t _M = M;

        Rep coeff1{};
        Rep coeff2{};

       public:
        LiePotential(Rep c1, Rep c2) : coeff1(c1), coeff2(c2) {}
    }

    template <typename Rep = double>
    using LJPotential = LiePotential<6, 12, Rep>;

}   // namespace mstd

#endif   // __MSTD__PHYSICS__POTENTIALS__LIE_POTENTIAL_HPP__