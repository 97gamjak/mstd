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

#ifndef __MSTD__TYPES__CACHE__CACHE_HPP__
#define __MSTD__TYPES__CACHE__CACHE_HPP__

#include <concepts>
#include <optional>
#include <tuple>

#include "cache_detail.hpp"

namespace mstd
{

    /**
     * @brief Concept to check if a type S satisfies the requirements of a
     * source: it names its domain(s), its value type and gives access to
     * the value.
     */
    template <typename S>
    concept Source =
        (requires { typename S::domains; } ||
         requires { typename S::domain; }) &&
        requires { typename S::value_type; } &&
        requires(const S& source) {
            { source.get() } -> std::same_as<const typename S::value_type&>;
        };

    /**
     * @brief A simple cache class that stores a value of type T and allows
     * for lazy computation of the value using a registered compute function.
     * The value is valid as long as no domain of its sources advanced.
     * (NOT thread-safe)
     */
    template <typename T, Source... Sources>
    class Cache
    {
       public:
        using Func    = T (*)(const typename Sources::value_type&...);
        using domains = detail::DomainsOf<Sources...>;

       private:
        static_assert(
            sizeof...(Sources) > 0,
            "a Cache needs at least one source"
        );

        mutable std::optional<T>         _value   = std::nullopt;
        Func                             _compute = nullptr;
        mutable typename domains::Epochs _epochs{};

       public:
        explicit Cache(Func compute);
        const T& get(const Sources&... sources) const;

       private:
        void _slow(const Sources&... sources) const;
    };

    /**
     * @brief A Cache that is bound to its sources at construction. It has a
     * get() without arguments, so it is itself a Source and can be the
     * source of another Cache or BoundCache.
     *
     * The sources are held by address: they must outlive the cache, and the
     * cache can neither be copied nor moved (an owner has to rebind in its
     * own copy/move operations).
     */
    template <typename T, Source... Sources>
    class BoundCache
    {
       public:
        using value_type = T;
        using Func       = typename Cache<T, Sources...>::Func;
        using domains    = typename Cache<T, Sources...>::domains;

       private:
        Cache<T, Sources...>          _cache;
        std::tuple<const Sources*...> _sources;

       public:
        BoundCache(Func compute, const Sources&... sources);

        BoundCache(const BoundCache&)            = delete;
        BoundCache& operator=(const BoundCache&) = delete;

        const T& get() const;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__CACHE__CACHE_TPP__
#include "cache.tpp"
#endif

#endif   // __MSTD__TYPES__CACHE__CACHE_HPP__
