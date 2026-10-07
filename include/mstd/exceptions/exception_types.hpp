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

#ifndef __MSTD__EXCEPTIONS__EXCEPTION_TYPES_HPP__
#define __MSTD__EXCEPTIONS__EXCEPTION_TYPES_HPP__

#include <cstdint>

#include "mstd/enum.hpp"

namespace mstd
{
#define EXCEPTION_TYPE_LIST(X) X(RuntimeError)

    MSTD_ENUM(ExceptionType, std::uint8_t, EXCEPTION_TYPE_LIST)

}   // namespace mstd

#endif   // __MSTD__EXCEPTIONS__EXCEPTION_TYPES_HPP__
