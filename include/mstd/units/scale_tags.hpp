#ifndef __MSTD_UNITS_SCALE_TAGS_HPP__
#define __MSTD_UNITS_SCALE_TAGS_HPP__

#include <ratio>

namespace mstd
{
    // clang-format off
    template <typename R> struct L { using ratio = R; using kind = void; };
    template <typename R> struct M { using ratio = R; using kind = void; };
    template <typename R> struct T { using ratio = R; using kind = void; };
    template <typename R> struct K { using ratio = R; using kind = void; };
    template <typename R> struct N { using ratio = R; using kind = void; };
    template <typename R> struct I { using ratio = R; using kind = void; };
    template <typename R> struct Cd { using ratio = R; using kind = void; };
    // clang-format on

    using Ang = L<std::ratio<1, 10>>;
    using nm  = L<std::nano>;
    using um  = L<std::micro>;
    using mm  = L<std::milli>;
    using m   = L<std::ratio<1>>;
    using km  = L<std::kilo>;

    using ng = M<std::pico>;
    using ug = M<std::nano>;
    using mg = M<std::micro>;
    using g  = M<std::milli>;
    using kg = M<std::ratio<1>>;

    using fs  = T<std::femto>;
    using ps  = T<std::pico>;
    using ns  = T<std::nano>;
    using us  = T<std::micro>;
    using ms  = T<std::milli>;
    using s   = T<std::ratio<1>>;
    using min = T<std::ratio<60>>;
    using h   = T<std::ratio<3600>>;
    using d   = T<std::ratio<86400>>;
    using yr  = T<std::ratio<31536000>>;
    // clang-format on

}   // namespace mstd

#endif   // __MSTD_UNITS_SCALE_TAGS_HPP__