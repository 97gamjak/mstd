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

#ifndef __MSTD__TYPE_TRAITS__STRING_HPP__
#define __MSTD__TYPE_TRAITS__STRING_HPP__

#include <concepts>
#include <string>

namespace mstd
{
    /**
     * @brief Concept checking whether Tag::toString(const T&) is a valid
     * expression that returns something convertible to std::string.
     *
     * @tparam Tag the tag type expected to provide a static toString member
     * @tparam T the value type passed to Tag::toString
     */
    template <typename Tag, typename T>
    concept HasToString = requires(const T &value) {
        { Tag::toString(value) } -> std::convertible_to<std::string>;
    };

    /**
     * @brief Case-insensitive string comparison.
     *
     * @param stringA the first string view
     * @param stringB the second string view
     * @return true if the strings are equal ignoring case, false otherwise
     */
    static constexpr bool iequals(
        std::string_view stringA,
        std::string_view stringB
    )
    {
        if (stringA.size() != stringB.size())
            return false;

        for (std::size_t i = 0; i < stringA.size(); ++i)
        {
            char characterA = stringA[i];
            char characterB = stringB[i];

            if (characterA >= 'A' && characterA <= 'Z')
                characterA = static_cast<char>(characterA + ('a' - 'A'));
            if (characterB >= 'A' && characterB <= 'Z')
                characterB = static_cast<char>(characterB + ('a' - 'A'));

            if (characterA != characterB)
                return false;
        }

        return true;
    }

    /**
     * @brief Checks if there are any case-insensitive collisions in the given
     * array of string views.
     *
     * @tparam size the size of the array
     * @param names the array of string views to check
     * @return true if there is a case-insensitive collision, false otherwise
     */
    template <std::size_t size>
    static constexpr bool hasCaseInsensitiveCollision(
        const std::array<std::string_view, size> &names
    )
    {
        for (std::size_t i = 0; i < size; ++i)
            for (std::size_t j = i + 1; j < size; ++j)
                if (iequals(names.at(i), names.at(j)))
                    return true;
        return false;
    }
}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__STRING_HPP__
