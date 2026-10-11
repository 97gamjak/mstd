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

#ifndef __MSTD__TYPES__CACHE_HPP__
#define __MSTD__TYPES__CACHE_HPP__

#include <array>
#include <optional>

#include "mstd/types/global_epoch.hpp"

namespace mstd
{
    /**
     * @brief Concept to check if a type S satisfies the requirements of a
     * source.
     */
    template <typename S>
    concept Source = requires(const S& source) {
        typename S::domain;
        typename S::value_type;
        { source.get() } -> std::same_as<const typename S::value_type&>;
    };

    /**
     * @brief A simple cache class that stores a value of type T and allows
     * for lazy computation of the value using a registered compute function.
     * (NOT thread-safe)
     */
    template <typename T, Source... Sources>
    class Cache
    {
       public:
        using Func = T (*)(const typename Sources::value_type&...);

       private:
        static constexpr std::size_t N = sizeof...(Sources);
        static_assert(N > 0, "a Cache needs at least one source");

        mutable std::optional<T>              _value   = std::nullopt;
        Func                                  _compute = nullptr;
        mutable std::array<EpochValueType, N> _versions{};

       public:
        explicit Cache(Func compute);
        const T& get(const Sources&... sources) const;

       private:
        bool _isUpToDate() const;

        void _slow(const Sources&... sources) const;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__CACHE_TPP__
#include "cache.tpp"
#endif

#endif   // __MSTD__TYPES__CACHE_HPP__
