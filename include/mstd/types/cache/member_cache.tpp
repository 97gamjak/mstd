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

#ifndef __MSTD__TYPES__MEMBER_CACHE_TPP__
#define __MSTD__TYPES__MEMBER_CACHE_TPP__

#include "member_cache.hpp"
#include "mstd/debugging.hpp"
#include "mstd/exceptions.hpp"

namespace mstd
{
    /**
     * @brief Reads the value of a source member of an owner.
     *
     * @param owner the object that holds the source
     * @param source the source member
     * @return const typename S::value_type& the value of the source
     */
    template <typename Owner, typename S>
    decltype(auto) detail::readSource(const Owner& owner, const S& source)
    {
        if constexpr (requires { source.get(owner); })
            return source.get(owner);
        else
            return source.get();
    }

    /**
     * @brief Constructs a MemberCache with the given compute function.
     *
     * @param compute The compute function used to lazily compute the cached
     * value.
     */
    template <typename T, typename Owner, auto... Members>
    MemberCache<T, Owner, Members...>::MemberCache(Func compute)
        : _compute(compute)
    {
    }

    /**
     * @brief Retrieves the cached value, computing it if necessary.
     *
     * @param owner the object that holds this cache and its sources
     * @return The cached value of type T.
     */
    template <typename T, typename Owner, auto... Members>
    const T& MemberCache<T, Owner, Members...>::get(const Owner& owner) const
    {
        if (_value && domains::isCurrent(_epochs))
            return *_value;

        MSTD_ASSERT_NOT_HOT("Cache miss: value is not current");

        _slow(owner);
        return *_value;
    }

    /**
     * @brief Recomputes the value and remembers the current domain epochs.
     *
     * @param owner the object that holds this cache and its sources
     */
    template <typename T, typename Owner, auto... Members>
    void MemberCache<T, Owner, Members...>::_slow(const Owner& owner) const
    {
        if (!_compute)
        {
            throw mstd::RuntimeError(
                "Cache value is not valid and no compute function is set."
            );
        }

        _value  = _compute(detail::readSource(owner, owner.*Members)...);
        _epochs = domains::now();
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__MEMBER_CACHE_TPP__
