#ifndef __MSTD__SHARED__TRAITS__GETTER_HPP__
#define __MSTD__SHARED__TRAITS__GETTER_HPP__

#include <type_traits>

namespace mstd::detail
{
    /**
     * Determines if a type should be passed by value.
     */
    template <typename T>
    inline constexpr bool pass_by_value_v =
        std::is_trivially_copyable_v<T> && sizeof(T) <= 2 * sizeof(void *);

    /**
     * Returns the value of a member, either by value or by const reference,
     * depending on the type.
     */
    template <typename T>
    [[nodiscard]] constexpr decltype(auto) getter_value(const T &v) noexcept
    {
        if constexpr (pass_by_value_v<T>)
            return T{v};   // returns T (by value)
        else
            return (v);   // returns const T&
    }

    template <typename T>
    using getter_t =
        std::conditional_t<detail::pass_by_value_v<T>, T, const T &>;
}   // namespace mstd::detail

#endif   // __MSTD__SHARED__TRAITS__GETTER_HPP__
