#ifndef __MSTD_ERROR_HPP__
#define __MSTD_ERROR_HPP__

#include <type_traits>

namespace mstd::error
{
    // clang-format off
    /**
     * @brief a struct that is always false
     * 
     * @tparam T 
     */
    template <typename T>
    struct always_false : std::false_type{};
    // clang-format on

}   // namespace mstd::error

#define MSTD_COMPILE_FAIL(msg) \
    static_assert(::mstd::error::always_false<void>::value, msg)

#endif   // __MSTD_ERROR_HPP__