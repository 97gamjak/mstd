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

#ifndef __COMPILE_TIME_ERRORS_HPP__
#define __COMPILE_TIME_ERRORS_HPP__

#include <type_traits>

/**
 * @file compile_time_errors.hpp
 * @brief Utilities for generating compile-time errors with custom messages.
 *
 * Provides a mechanism to trigger static assertions with user-defined error
 * messages, useful for enforcing constraints in template metaprogramming.
 */

namespace mstd::error
{
    // clang-format off
    /**
     * @brief a struct that is always false
     * 
     * @tparam T 
     */
    template <typename T>
    struct always_false : std::false_type{};
    // clang-format on

}   // namespace mstd::error

#define MSTD_COMPILE_FAIL(msg) \
    static_assert(::mstd::error::always_false<void>::value, msg)

#endif   // __COMPILE_TIME_ERRORS_HPP__