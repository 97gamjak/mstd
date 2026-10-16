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

#ifndef __MSTD__TYPES__CACHE__SYNC_CACHE_HPP__
#define __MSTD__TYPES__CACHE__SYNC_CACHE_HPP__

#include <atomic>
#include <mutex>
#include <optional>
#include <type_traits>

#include "member_cache.hpp"

namespace mstd
{
    /**
     * @brief The thread-safe counterpart of MemberCache: it names its
     * sources the same way and is valid as long as no domain of its sources
     * advanced, but get() may be called from several threads at once.
     *
     * @details Concurrent get() calls are safe: a stale value is computed
     * exactly once and all callers receive it. A hit is lock-free and only
     * reads shared state, so it scales with the number of threads. The value
     * is returned by copy, since a reference could be invalidated by another
     * thread's recompute.
     *
     * The sources themselves are not protected: they must not be written
     * while any thread reads (set the properties in a serial section, and
     * separate it from the readers by a barrier, join or similar). The
     * compute function must not call get() on its own cache. A SyncCache
     * can be a source of a SyncCache or a MemberCache and vice versa.
     *
     * Copying an owner copies the cached state under the source's lock; the
     * mutex itself is never copied, so the owner keeps its defaulted copy
     * and assignment.
     */
    template <typename T, typename Owner, auto... Members>
    class SyncCache
    {
        static_assert(
            sizeof...(Members) > 0,
            "a SyncCache needs at least one source"
        );
        static_assert(
            (std::is_member_object_pointer_v<decltype(Members)> && ...),
            "the sources of a SyncCache must be pointers to data members"
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
        // _value is written only under _mutex and published by _filled and
        // _epochs (release); a hit reads it after acquiring both
        mutable std::optional<T>               _value   = std::nullopt;
        Func                                   _compute = nullptr;
        mutable std::atomic<bool>              _filled{false};
        mutable typename domains::AtomicEpochs _epochs{};
        mutable std::mutex                     _mutex;

       public:
        explicit SyncCache(Func compute);

        SyncCache(const SyncCache& other);
        SyncCache& operator=(const SyncCache& other);
        ~SyncCache() = default;

        [[nodiscard]] T get(const Owner& owner) const;

       private:
        static void _copyEpochs(
            typename domains::AtomicEpochs&       to,
            const typename domains::AtomicEpochs& from
        );
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__CACHE__SYNC_CACHE_TPP__
#include "sync_cache.tpp"
#endif

#endif   // __MSTD__TYPES__CACHE__SYNC_CACHE_HPP__
