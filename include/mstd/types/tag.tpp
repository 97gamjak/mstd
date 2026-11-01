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

#ifndef __MSTD__TYPES__TAG_TPP__
#define __MSTD__TYPES__TAG_TPP__

#include <algorithm>
#include <cstddef>

#include "tag.hpp"

namespace mstd
{
    /**
     * @brief Construct a new Tag object
     *
     * @param str The string literal to initialize the tag with
     */
    template <size_t N>
    constexpr Tag<N>::Tag(const char (&str)[N])
    {
        std::copy_n(str, N, value);
    }

    /**
     * @brief Returns a string view of the tag's value
     *
     * @return constexpr std::string_view
     */
    template <size_t N>
    constexpr std::string_view Tag<N>::view() const
    {
        return {value, N - 1};
    }

    /**
     * @brief Converts the tag's value to a std::string
     *
     * @return std::string
     */
    template <size_t N>
    std::string Tag<N>::to_string() const
    {
        return std::string(value, N - 1);
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__TAG_TPP__
