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

#ifndef __MSTD__STRING__JOIN_HPP__
#define __MSTD__STRING__JOIN_HPP__

#include <ranges>
#include <string>

#include "mstd/type_traits/ranges_traits.hpp"

namespace mstd
{
    /**
     * @brief Join a range of strings into a single string with a delimiter
     *
     * @tparam R
     * @param r
     * @param delim
     * @return std::string
     */
    template <joinable_range R>
    std::string join(R&& r, std::string_view delim)
    {
        auto joined = std::ranges::views::join_with(std::forward<R>(r), delim);

        return {joined.begin(), joined.end()};
    }

    /**
     * @brief overload of join that uses an empty string as delimiter
     *
     * @tparam R
     * @param r
     * @return std::string
     */
    template <joinable_range R>
    std::string join(R&& r)
    {
        return join(std::forward<R>(r), "");
    }

}   // namespace mstd

#endif   // __MSTD__STRING__JOIN_HPP__