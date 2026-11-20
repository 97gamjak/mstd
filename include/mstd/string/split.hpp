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

#ifndef __MSTD__STRING__SPLIT_HPP__
#define __MSTD__STRING__SPLIT_HPP__

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace mstd
{
    /**
     * @brief Split a string into parts at each occurrence of a delimiter and
     * transform each part
     *
     * Empty parts are kept (e.g. "a,,b" -> {"a", "", "b"}). An empty input
     * yields a single empty part. An empty delimiter yields the whole input
     * as a single part.
     *
     * @tparam T result type of the transformation
     * @param str The string to split
     * @param delim The delimiter to split at
     * @param transform A function to transform each part into a T
     * @return std::vector<T>
     */
    template <typename T>
    std::vector<T> split(
        std::string_view                        str,
        std::string_view                        delim,
        std::function<T(std::string_view)>      transform
    )
    {
        std::vector<T> result;

        if (delim.empty())
        {
            result.push_back(transform(str));
            return result;
        }

        std::size_t start = 0;
        while (true)
        {
            const auto pos = str.find(delim, start);
            if (pos == std::string_view::npos)
            {
                result.push_back(transform(str.substr(start)));
                break;
            }

            result.push_back(transform(str.substr(start, pos - start)));
            start = pos + delim.size();
        }

        return result;
    }

    /**
     * @brief Split a string into parts at each occurrence of a delimiter
     *
     * @param str The string to split
     * @param delim The delimiter to split at
     * @return std::vector<std::string>
     */
    inline std::vector<std::string> split(
        std::string_view str,
        std::string_view delim
    )
    {
        return split<std::string>(
            str,
            delim,
            [](std::string_view s) { return std::string(s); }
        );
    }

}   // namespace mstd

#endif   // __MSTD__STRING__SPLIT_HPP__
