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

#ifndef __MSTD__TYPES__ID_HPP__
#define __MSTD__TYPES__ID_HPP__

namespace mstd
{

    struct DefaultTag
    {
    };

    /**
     * @brief A class representing a unique identifier of type T.
     *
     * @tparam T The type of the unique identifier.
     */
    template <typename T, typename Tag = DefaultTag>
    class Id
    {
       private:
        T _value;

       public:
        Id() = default;
        explicit Id(T v);

        static Id next();

        constexpr bool operator==(const Id& other) const noexcept = default;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__ID_TPP__
#include "id.tpp"
#endif

#endif   // __MSTD__TYPES__ID_HPP__
