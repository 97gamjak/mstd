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

#ifndef __MSTD__FILE__FILE_IMPL__HPP__
#define __MSTD__FILE__FILE_IMPL__HPP__

#include <filesystem>
#include <fstream>
#include <string>

#include "file.hpp"

namespace mstd
{
    /**
     * @brief constructs a File object with the given file name
     *
     * @param fileName the name of the file
     */
    inline File::File(std::string fileName) : _fileName(std::move(fileName)) {}

    /**
     * @brief checks if a file exists and can be opened
     *
     * @return true if file exists and can be opened
     * @return false if file does not exist or cannot be opened
     */
    inline bool File::exists() const
    {
        if (!std::filesystem::is_regular_file(_fileName))
            return false;

        std::ifstream file(_fileName);
        return file.good();
    }

    /**
     * @brief returns the file name of the File object
     *
     * @return the file name
     */
    inline std::string File::fileName() const { return _fileName; }
}   // namespace mstd

#endif   // __MSTD__FILE__FILE_IMPL__HPP__
