#ifndef __MSTD__TYPES__STRONG_TYPE_ALIASES_HPP__
#define __MSTD__TYPES__STRONG_TYPE_ALIASES_HPP__

#include "strong_type.hpp"

namespace mstd
{
    /**
     * @brief Alias template for a strongly-typed size_t with a specific tag.
     *
     * @tparam Tag The tag type to differentiate this strong type.
     */
    template <typename Tag>
    using StrongSizeT = StrongType<
        size_t,
        Tag,
        StrongTypeTrait::ORDERED | StrongTypeTrait::HASHABLE |
            StrongTypeTrait::INCREMENT | StrongTypeTrait::ARITHMETIC>;
}   // namespace mstd

#endif   // __MSTD__TYPES__STRONG_TYPE_ALIASES_HPP__
