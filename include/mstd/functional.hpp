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

#ifndef __FUNCTIONAL_HPP__
#define __FUNCTIONAL_HPP__

namespace mstd
{
    /**
     * @brief Functor for addition.
     * @note Used in pack operations.
     */
    struct add_fn
    {
        constexpr int operator()(int a, int b) const { return a + b; }
    };

    /**
     * @brief Functor for subtraction.
     * @note Used in pack operations.
     */
    struct sub_fn
    {
        constexpr int operator()(int a, int b) const { return a - b; }
    };

    /**
     * @brief Functor for scaling (multiplying) by a constant factor.
     * @note Used in pack operations.
     * @tparam K The scaling factor.
     */
    template <int K>
    struct scale_fn
    {
        constexpr int operator()(int e) const { return e * K; }
    };

}   // namespace mstd

#endif   // __FUNCTIONAL_HPP__