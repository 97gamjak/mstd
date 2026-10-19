#ifndef __MSTD__MATH__ANGULAR_HPP__
#define __MSTD__MATH__ANGULAR_HPP__

#include <cmath>

namespace mstd::detail
{
    // Tag type: makes ADL search mstd::detail, where the *_impl hooks live
    struct adl_tag
    {
    };

    using std::cos;
    using std::sin;
    using std::tan;

    // fallback for arithmetic types
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto sin_impl(const T &x, adl_tag)
    {
        return sin(x);
    }
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto cos_impl(const T &x, adl_tag)
    {
        return cos(x);
    }
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto tan_impl(const T &x, adl_tag)
    {
        return tan(x);
    }
}   // namespace mstd::detail

namespace mstd
{
    // unqualified call: ADL (through adl_tag) finds the hooks, including
    // the ones units.hpp adds later
    template <typename T>
    constexpr auto sin(const T &x)
    {
        return sin_impl(x, detail::adl_tag{});
    }
    template <typename T>
    constexpr auto cos(const T &x)
    {
        return cos_impl(x, detail::adl_tag{});
    }
    template <typename T>
    constexpr auto tan(const T &x)
    {
        return tan_impl(x, detail::adl_tag{});
    }
}   // namespace mstd

#undef MSTD_MATH_UNARY

#endif   // __MSTD__MATH__ANGULAR_HPP__
