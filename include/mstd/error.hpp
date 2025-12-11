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

#ifndef __MSTD_ERROR_HPP__
#define __MSTD_ERROR_HPP__

#include <type_traits>

#include "compile.hpp"

namespace mstd
{
    /**
     * @brief a struct that is always false
     *
     * @tparam T
     */
    template <typename T>
    struct always_false : std::false_type
    {
    };

}   // namespace mstd

#define MSTD_COMPILE_FAIL(msg) \
    static_assert(::mstd::always_false<void>::value, msg);

#define MSTD_WARN_BUGGY_LIBRARY(library_name)
#define MSTD_WARN_BUGGY_HEADER(header_file)

#if !MSTD_IGNORE_BUGGY_CODE

#undef MSTD_WARN_BUGGY_LIBRARY
#undef MSTD_WARN_BUGGY_HEADER

#define MSTD_WARN_BUGGY_LIBRARY(library_name)                             \
    namespace mstd::buggy                                                 \
    {                                                                     \
        [[deprecated("Buggy library: " library_name " — don't use it!")]] \
        inline int _  = 0;                                                \
        inline int __ = _;                                                \
    }   // namespace mstd::buggy

#define MSTD_WARN_BUGGY_HEADER(header_file)                             \
    namespace mstd::buggy                                               \
    {                                                                   \
        [[deprecated("Buggy header: " header_file " — don't use it!")]] \
        inline int _  = 0;                                              \
        inline int __ = _;                                              \
    }   // namespace mstd::buggy

#endif

#endif   // __MSTD_ERROR_HPP__