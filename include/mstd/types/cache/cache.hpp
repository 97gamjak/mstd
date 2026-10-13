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
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "mstd/types/global_epoch.hpp"

namespace mstd
{
    namespace detail
    {
        /**
         * @brief A compile-time list of epoch domains.
         */
        template <typename... Ds>
        struct DomainList
        {
            using Epochs = std::array<std::uint64_t, sizeof...(Ds)>;

            /// The current epoch of every domain in the list.
            static Epochs now() noexcept { return {GlobalEpoch<Ds>::now()...}; }

            /// True if no domain in the list advanced since `epochs`.
            static bool isCurrent(const Epochs& epochs) noexcept
            {
                return [&]<std::size_t... I>(std::index_sequence<I...>)
                {
                    return ((epochs[I] == GlobalEpoch<Ds>::now()) && ...);
                }(std::index_sequence_for<Ds...>{});
            }

            using AtomicEpochs =
                std::array<std::atomic<std::uint64_t>, sizeof...(Ds)>;

            static bool isCurrent(const AtomicEpochs& epochs) noexcept
            {
                return [&]<std::size_t... I>(std::index_sequence<I...>)
                {
                    return (
                        (epochs[I].load(std::memory_order_acquire) ==
                         GlobalEpoch<Ds>::now()) &&
                        ...
                    );
                }(std::index_sequence_for<Ds...>{});
            }

            static void publish(AtomicEpochs& epochs) noexcept
            {
                [&]<std::size_t... I>(std::index_sequence<I...>)
                {
                    (epochs[I].store(
                         GlobalEpoch<Ds>::now(),
                         std::memory_order_release
                     ),
                     ...);
                }(std::index_sequence_for<Ds...>{});
            }
        };

        template <typename List, typename D>
        struct AppendUnique;

        template <typename... Ds, typename D>
        struct AppendUnique<DomainList<Ds...>, D>
        {
            using type = std::conditional_t<
                (std::is_same_v<D, Ds> || ...),
                DomainList<Ds...>,
                DomainList<Ds..., D>>;
        };

        template <typename List, typename Other>
        struct Merge;

        template <typename List>
        struct Merge<List, DomainList<>>
        {
            using type = List;
        };

        template <typename List, typename D, typename... Rest>
        struct Merge<List, DomainList<D, Rest...>>
        {
            using type = typename Merge<
                typename AppendUnique<List, D>::type,
                DomainList<Rest...>>::type;
        };

        template <typename List, typename... Lists>
        struct MergeAll
        {
            using type = List;
        };

        template <typename List, typename First, typename... Rest>
        struct MergeAll<List, First, Rest...>
        {
            using type =
                typename MergeAll<typename Merge<List, First>::type, Rest...>::
                    type;
        };

        /// The domains a source depends on: `S::domains` if it has one
        /// (a derived cache), else the single `S::domain` (a property).
        template <typename S>
        consteval auto sourceDomains()
        {
            if constexpr (requires { typename S::domains; })
                return typename S::domains{};
            else
                return DomainList<typename S::domain>{};
        }

        /// All domains of a set of sources, each listed once.
        template <typename... Ss>
        using DomainsOf =
            typename MergeAll<DomainList<>, decltype(sourceDomains<Ss>())...>::
                type;
    }   // namespace detail

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
        BoundCache(Func compute, const Sources&... sources)
            : _cache(compute), _sources(&sources...)
        {
        }

        BoundCache(const BoundCache&)            = delete;
        BoundCache& operator=(const BoundCache&) = delete;

        const T& get() const
        {
            return std::apply(
                [this](const Sources*... sources) -> const T&
                { return _cache.get(*sources...); },
                _sources
            );
        }
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__CACHE_TPP__
#include "cache.tpp"
#endif

#endif   // __MSTD__TYPES__CACHE_HPP__
