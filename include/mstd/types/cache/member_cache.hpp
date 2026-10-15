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

#ifndef __MSTD__TYPES__MEMBER_CACHE_HPP__
#define __MSTD__TYPES__MEMBER_CACHE_HPP__

#include <optional>
#include <type_traits>

#include "cache_detail.hpp"

namespace mstd
{
    namespace detail
    {
        /**
         * @brief Extracts the member type from a pointer to a data member.
         *
         * @detail Works while the owning class is still incomplete, which
         * is the case when the cache itself is a member of that class.
         */
        template <typename Pointer>
        struct MemberTraits;

        template <typename X, typename C>
        struct MemberTraits<X C::*>
        {
            using type = X;
        };

        template <auto Member>
        using MemberTypeT = typename MemberTraits<decltype(Member)>::type;

        /**
         * @brief Concept to check if a type can be a source of a MemberCache:
         * it names its domain(s) and its value type.
         */
        template <typename S>
        concept MemberSource =
            (requires { typename S::domains; } ||
             requires { typename S::domain; }) &&
            requires { typename S::value_type; };

        /**
         * @brief Reads the value of a source member of an owner.
         *
         * @detail A property is read with get(), another MemberCache with
         * get(owner).
         */
        template <typename Owner, typename S>
        decltype(auto) readSource(const Owner& owner, const S& source);
    }   // namespace detail

    /**
     * @brief A cache that lives as a member of an owner class and names its
     * sources as pointers to other data members of that owner (properties
     * or other MemberCaches).
     *
     * @detail The sources are part of the type, so nothing is stored that
     * points into the owner: the owner keeps its defaulted copy, move and
     * assignment, no call site can pass a wrong source and the compute
     * function states in its signature what it reads. The value is valid as
     * long as no domain of its sources advanced. Chains of MemberCaches
     * recompute only what is asked for. (NOT thread-safe)
     *
     * @tparam T type of the cached value
     * @tparam Owner class that holds the cache and its sources
     * @tparam Members pointers to the source data members of Owner, in the
     * order of the arguments of the compute function
     */
    template <typename T, typename Owner, auto... Members>
    class MemberCache
    {
        static_assert(
            sizeof...(Members) > 0,
            "a MemberCache needs at least one source"
        );
        static_assert(
            (std::is_member_object_pointer_v<decltype(Members)> && ...),
            "the sources of a MemberCache must be pointers to data members"
        );
        static_assert(
            (detail::MemberSource<detail::MemberTypeT<Members>> && ...),
            "every source needs domain(s) and value_type"
        );

       public:
        using value_type = T;
        using domains    = detail::DomainsOf<detail::MemberTypeT<Members>...>;
        using Func =
            T (*)(const typename detail::MemberTypeT<Members>::value_type&...);

       private:
        mutable std::optional<T>         _value   = std::nullopt;
        Func                             _compute = nullptr;
        mutable typename domains::Epochs _epochs{};

       public:
        MemberCache() = delete;
        explicit MemberCache(Func compute);

        [[nodiscard]] const T& get(const Owner& owner) const;

       private:
        void _slow(const Owner& owner) const;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__MEMBER_CACHE_TPP__
#include "member_cache.tpp"
#endif

#endif   // __MSTD__TYPES__MEMBER_CACHE_HPP__
