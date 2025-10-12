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

#ifndef __MSTD_VECTOR_SVEC_HPP__
#define __MSTD_VECTOR_SVEC_HPP__

#include <array>
#include <cstddef>
#include <format>

#include "mstd/error/exceptions.hpp"

namespace mstd::vec
{

    template <size_t N, typename Rep = double>
    class SVec
    {
        static_assert(std::is_trivially_copyable_v<Rep>);

       private:
        std::array<Rep, N> _data;

       public:
        static consteval size_t size() { return N; }

        constexpr SVec() = default;
        explicit constexpr SVec(std::array<Rep, N>& arr) : _data(arr) {}
        explicit constexpr SVec(const Rep val) : _data{} { _data.fill(val); }
        explicit constexpr SVec(std::initializer_list<Rep> list);

        constexpr SVec(const SVec&) = default;
    };

    template <size_t N, typename Rep>
    constexpr SVec<N, Rep>::SVec(std::initializer_list<Rep> list) : _data{}
    {
        if (list.size() != N)
            throw mstd::error::DIMENSION_MISMATCH(
                std::format(
                    "SVec: initializer list size {} does not match vector size "
                    "{}",
                    list.size(),
                    N
                )
            );

        std::copy(list.begin(), list.end(), _data.begin());
    }

}   // namespace mstd::vec

#endif   // __MSTD_VECTOR_SVEC_HPP__