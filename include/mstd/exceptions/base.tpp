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

#ifndef __MSTD__EXCEPTIONS__BASE_TPP__
#define __MSTD__EXCEPTIONS__BASE_TPP__

#include "base.hpp"

namespace mstd
{
    /**
     * @brief Constructs a new BaseException object with the given message.
     *
     * @param message The exception message.
     */
    template <ExceptionType T>
    BaseException<T>::BaseException(const std::string& message)
        : std::exception(), _message(message)
    {
    }

    /**
     * @brief Retrieves the exception message.
     *
     * @return The exception message as a C-style string.
     */
    template <ExceptionType T>
    const char* BaseException<T>::what() const noexcept
    {
        static const auto msg = ExceptionTypeMeta::toString(T) + _message;
        return msg.c_str();
    }
}   // namespace mstd

#endif   // __MSTD__EXCEPTIONS__BASE_TPP__
