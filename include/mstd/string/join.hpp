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

#include <format>
#include <functional>
#include <ranges>
#include <string>

#include "mstd/type_traits/ranges_traits.hpp"

namespace mstd
{
    /**
     * @brief Join a range of strings into a single string with a delimiter and
     * an optional transformation function
     *
     * @tparam R
     * @param r
     * @param delim The delimiter to insert between elements of the range
     * @param transform A function to transform each element of the range into a
     * string
     * @return std::string
     */
    template <std::ranges::input_range R>
    std::string join(
        R&&                                                       r,
        std::string_view                                          delim,
        std::function<std::string(std::ranges::range_value_t<R>)> transform
    )
    {
        auto joined = std::ranges::views::join_with(
            std::ranges::views::transform(std::forward<R>(r), transform),
            delim
        );

        return std::ranges::to<std::string>(joined);
    }

    /**
     * @brief Join a range of strings into a single string with a delimiter
     *
     * @tparam R
     * @param r
     * @param delim
     * @return std::string
     */
    template <std::ranges::input_range R>
    std::string join(R&& r, std::string_view delim)
    {
        return join(
            std::forward<R>(r),
            delim,
            [](auto&& s) -> std::string { return std::format("{}", s); }
        );
    }

    /**
     * @brief overload of join that uses an empty string as delimiter
     *
     * @tparam R
     * @param r
     * @return std::string
     */
    template <std::ranges::input_range R>
    std::string join(R&& r)
    {
        return join(std::forward<R>(r), "");
    }

}   // namespace mstd

#endif   // __MSTD__STRING__JOIN_HPP__
