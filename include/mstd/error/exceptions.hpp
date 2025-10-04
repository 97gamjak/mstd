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

#ifndef __MSTD_EXCEPTIONS_HPP__
#define __MSTD_EXCEPTIONS_HPP__

#include <source_location>
#include <sstream>
#include <stdexcept>

/**
 * @file exceptions.hpp
 * @brief Custom exception types for the mstd library.
 *
 * Defines `MSTD_Exception`, a runtime exception that captures source location
 * information to aid in debugging. This exception can be used throughout the
 * library to provide detailed error messages.
 */

namespace mstd::error
{
    /**
     * @brief A custom exception that includes source location information.
     *
     * Inherits from `std::runtime_error` and captures the file name, line
     * number, and function name where the exception was thrown.
     */
    class MSTD_Exception : public std::runtime_error
    {
       private:
        std::source_location _where;

        /**
         * @brief Helper to format the error message with location info.
         *
         * @param msg The original error message.
         * @param where The source location where the exception was created.
         * @return std::string The formatted error message.
         */
        static std::string add_location(
            const std::string&          msg,
            const std::source_location& where
        )
        {
            std::ostringstream oss;
            oss << "\n[MSTD LIB] " << msg << " (at " << where.file_name() << ":"
                << where.line() << " in " << where.function_name() << ")";
            return oss.str();
        }

       public:
        /**
         * @brief Construct a new MSTD_Exception object.
         *
         * @param msg The error message.
         * @param where The source location (default: current location).
         */
        explicit MSTD_Exception(
            const std::string&         msg,
            const std::source_location where = std::source_location::current()
        )
            : std::runtime_error(add_location(msg, where)), _where(where)
        {
        }

        /**
         * @brief Get the source location where the exception was created.
         *
         * @return const std::source_location& The source location.
         */
        const std::source_location& where() const noexcept { return _where; }
    };

    /**
     * @brief Exception for dimension mismatch errors.
     *
     * Inherits from `MSTD_Exception` and is thrown when operations involve
     * incompatible units or dimensions.
     */
    class DIMENSION_MISMATCH : public MSTD_Exception
    {
       public:
        /**
         * @brief Construct a new DIMENSION_MISMATCH object.
         * @param msg The error message.
         */
        explicit DIMENSION_MISMATCH(const std::string& msg)
            : MSTD_Exception("Dimension mismatch:\n" + msg)
        {
        }
    };

}   // namespace mstd::error

#endif   // __MSTD_EXCEPTIONS_HPP__