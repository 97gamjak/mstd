#ifndef __MSTD__TYPES__MEMBER_CACHE_TPP__
#define __MSTD__TYPES__MEMBER_CACHE_TPP__

#include "member_cache.hpp"
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
    const typename S::value_type& detail::readSource(
        const Owner& owner,
        const S&     source
    )
    {
        if constexpr (requires {
                          {
                              source.get(owner)
                          } -> std::same_as<const typename S::value_type&>;
                      })
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
