#ifndef __MSTD__TYPES__VERSION_HPP__
#define __MSTD__TYPES__VERSION_HPP__

#include "mstd/types/strong_type_aliases.hpp"

namespace mstd
{
    /**
     * @brief Tag type for versioning.
     *
     */
    struct VersionTag
    {
        static std::string toString() { return "VersionTag"; }
    };

    /**
     * @brief Strongly-typed version number.
     */
    using Version = StrongSizeT<VersionTag>;

}   // namespace mstd

#endif   // __MSTD__TYPES__VERSION_HPP__
