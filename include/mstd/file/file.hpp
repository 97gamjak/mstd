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

#ifndef __MSTD__FILE__FILE_HPP__
#define __MSTD__FILE__FILE_HPP__

#include <string>

namespace mstd
{
    /**
     * @brief Represents a file in the filesystem.
     */
    class File
    {
       private:
        std::string _fileName;

       public:
        explicit File(std::string fileName);

        [[nodiscard]]
        bool exists() const;

        [[nodiscard]]
        std::string fileName() const;

        [[nodiscard]]
        constexpr bool operator==(const File &other) const = default;
    };
}   // namespace mstd

#ifndef __MSTD__FILE__FILE_IMPL__HPP__
#include "file_impl.hpp"   // IWYU pragma: export
#endif

#endif   // __MSTD__FILE__FILE_HPP__
