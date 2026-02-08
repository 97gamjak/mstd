#ifndef __TYPE_TRAITS__ENUM_TRAITS_HPP__
#define __TYPE_TRAITS__ENUM_TRAITS_HPP__

#include <type_traits>

// NOTE: here we omit the namespace mstd to avoid problems with ADL
// because the whole mstd enum macro definition relies on the enum_meta struct
// being in the same namespace as the enum itself

/**
 * @brief A traits class for enums defined with MSTD_ENUM
 *
 * This class is specialized for each enum defined with MSTD_ENUM, providing
 * metadata about the enum such as its underlying type, values, and names.
 *
 * @tparam E The enum type
 */
template <typename E>
struct enum_meta;

/**
 * @brief A helper type alias to extract the enum metadata type
 *
 * @tparam E The enum type
 */
template <typename E>
using enum_meta_t = typename enum_meta<E>::type;

/**
 * @brief A concept to check if a type has enum metadata
 *
 * This concept checks if a type is an enum and has a corresponding
 * enum_meta specialization, which is true for enums defined with MSTD_ENUM.
 *
 * @tparam E The type to check
 */
template <typename E>
concept has_enum_meta =
    std::is_enum_v<E> && requires { typename enum_meta_t<E>; };

#endif   // __TYPE_TRAITS__ENUM_TRAITS_HPP__