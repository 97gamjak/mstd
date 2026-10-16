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

#ifndef __MSTD__TYPES__STRONG_TYPE_ALIASES_HPP__
#define __MSTD__TYPES__STRONG_TYPE_ALIASES_HPP__

#include "strong_type.hpp"

namespace mstd
{
    /**
     * @brief Alias template for a strongly-typed size_t with a specific tag.
     *
     * @tparam Tag The tag type to differentiate this strong type.
     */
    template <typename Tag>
    using StrongSizeT = StrongType<
        size_t,
        Tag,
        StrongTypeTrait::ORDERED | StrongTypeTrait::HASHABLE |
            StrongTypeTrait::INCREMENT | StrongTypeTrait::ARITHMETIC>;
}   // namespace mstd

#endif   // __MSTD__TYPES__STRONG_TYPE_ALIASES_HPP__
