#ifndef __MSTD__TYPES__STAMP_HPP__
#define __MSTD__TYPES__STAMP_HPP__

#include "mstd/types/id.hpp"
#include "mstd/types/version.hpp"

namespace mstd
{
    /**
     * @brief Strongly-typed stamp for a given type T.
     */
    template <typename T>
    struct Stamp
    {
        Id<T>          id;
        Version        version;
        constexpr bool operator==(const Stamp& other) const noexcept = default;
    };
}   // namespace mstd

#endif   // __MSTD__TYPES__STAMP_HPP__
