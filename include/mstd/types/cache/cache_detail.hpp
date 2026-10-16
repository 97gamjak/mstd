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

#ifndef __MSTD__TYPES__CACHE__CACHE_DETAIL_HPP__
#define __MSTD__TYPES__CACHE__CACHE_DETAIL_HPP__

#include <array>
#include <atomic>
#include <cstdint>

namespace mstd::detail
{
    /**
     * @brief A compile-time list of epoch domains.
     */
    template <typename... Ds>
    struct DomainList
    {
        using Epochs = std::array<std::uint64_t, sizeof...(Ds)>;
        using AtomicEpochs =
            std::array<std::atomic<std::uint64_t>, sizeof...(Ds)>;

        static Epochs now() noexcept;
        static bool   isCurrent(const Epochs& epochs) noexcept;
        static bool   isCurrent(const AtomicEpochs& epochs) noexcept;
        static void   publish(AtomicEpochs& epochs) noexcept;
    };

    /**
     * @brief Append a domain to a domain list if it is not already present.
     *
     * @tparam List The original domain list.
     * @tparam D The domain to append.
     */
    template <typename List, typename D>
    struct AppendUnique;

    /**
     * @brief Specialization of `AppendUnique` for a domain list and a domain.
     *
     * @tparam Ds The existing domains in the list.
     * @tparam D The domain to append if not already present.
     */
    template <typename... Ds, typename D>
    struct AppendUnique<DomainList<Ds...>, D>
    {
        using type = std::conditional_t<
            (std::is_same_v<D, Ds> || ...),
            DomainList<Ds...>,
            DomainList<Ds..., D>>;
    };

    /**
     * @brief Merge two domain lists, ensuring each domain appears only once.
     *
     * @tparam List The first domain list.
     * @tparam Other The second domain list.
     */
    template <typename List, typename Other>
    struct Merge;

    /**
     * @brief Specialization of `Merge` when the second domain list is empty.
     *
     * @tparam List The first domain list.
     */
    template <typename List>
    struct Merge<List, DomainList<>>
    {
        using type = List;
    };

    /**
     * @brief Specialization of `Merge` when the second domain list is
     * non-empty.
     *
     * @tparam List The first domain list.
     * @tparam D The first domain of the second list.
     * @tparam Rest The remaining domains of the second list.
     */
    template <typename List, typename D, typename... Rest>
    struct Merge<List, DomainList<D, Rest...>>
    {
        using type = typename Merge<
            typename AppendUnique<List, D>::type,
            DomainList<Rest...>>::type;
    };

    /**
     * @brief Merge all domain lists into a single domain list, ensuring each
     * domain appears only once.
     *
     * @tparam List The first domain list.
     * @tparam Lists The remaining domain lists to merge.
     */
    template <typename List, typename... Lists>
    struct MergeAll
    {
        using type = List;
    };

    /**
     * @brief Specialization of `MergeAll` when there are multiple domain lists
     * to merge.
     *
     * @tparam List The first domain list.
     * @tparam First The next domain list to merge.
     * @tparam Rest The remaining domain lists to merge.
     */
    template <typename List, typename First, typename... Rest>
    struct MergeAll<List, First, Rest...>
    {
        using type =
            typename MergeAll<typename Merge<List, First>::type, Rest...>::type;
    };

    /**
     * @brief Get the source domains of a source type `S`.
     *
     * @tparam S The source type.
     * @return The domain list of the source.
     */
    template <typename S>
    consteval auto sourceDomains()
    {
        if constexpr (requires { typename S::domains; })
            return typename S::domains{};
        else
            return DomainList<typename S::domain>{};
    }

    /**
     * @brief Get the combined domains of a set of sources, each listed once.
     *
     * @tparam Ss The source types.
     * @return The merged domain list of all sources.
     */
    template <typename... Ss>
    using DomainsOf =
        typename MergeAll<DomainList<>, decltype(sourceDomains<Ss>())...>::type;

}   // namespace mstd::detail

#ifndef __MSTD__TYPES__CACHE__CACHE_DETAIL_TPP__
#include "cache_detail.tpp"
#endif

#endif   // __MSTD__TYPES__CACHE__CACHE_DETAIL_HPP__
