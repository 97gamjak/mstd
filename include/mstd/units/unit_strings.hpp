#ifndef __MSTD_UNITS_STRINGS_HPP__
#define __MSTD_UNITS_STRINGS_HPP__

#include <ratio>
#include <string>

namespace mstd
{
    // clang-format off
    template <typename Ratio>
    constexpr std::string_view get_prefix(){return "ERROR";}
    template <>
    constexpr std::string_view get_prefix<std::ratio<1>>() { return ""; }
    template<>
    constexpr std::string_view get_prefix<std::femto>() { return "f"; }
    template <>
    constexpr std::string_view get_prefix<std::pico>() { return "p"; }
    template <>
    constexpr std::string_view get_prefix<std::nano>() { return "n"; }
    template <>
    constexpr std::string_view get_prefix<std::micro>() { return "u"; }
    template <>
    constexpr std::string_view get_prefix<std::milli>() { return "m"; }
    template <>
    constexpr std::string_view get_prefix<std::kilo>() { return "k"; }
    template <>
    constexpr std::string_view get_prefix<std::mega>() { return "M"; }
    template <>
    constexpr std::string_view get_prefix<std::giga>() { return "G"; }
    template <>
    constexpr std::string_view get_prefix<std::tera>() { return "T"; }
    //clang-format on

    template<typename Dim, typename Ratio>
    constexpr std::string get_unit_name_sub(const int id, const std::string base_unit){
        std::string unit = "";
        if (Dim ::exponents[id] != 0){
            unit += get_prefix<Ratio>();
            unit += base_unit;
            if (Dim ::exponents[id] != 1){
                unit += "^" + std::to_string(Dim::exponents[id]);
            }
        }
        return unit;
    }

    template<typename Dim, typename Pack>
    constexpr std::string get_unit_name(){
        std::string unit = "";
        unit += get_unit_name_sub<Dim, typename Pack::L::ratio>(0, "m");
        unit += get_unit_name_sub<Dim, typename Pack::M::ratio>(1, "g");
        unit += get_unit_name_sub<Dim, typename Pack::T::ratio>(2, "s");
        unit += get_unit_name_sub<Dim, typename Pack::K::ratio>(3, "K");
        unit += get_unit_name_sub<Dim, typename Pack::N::ratio>(4, "mol");
        unit += get_unit_name_sub<Dim, typename Pack::I::ratio>(5, "A");
        unit += get_unit_name_sub<Dim, typename Pack::Cd::ratio>(6, "Cd");
        return unit;
    }


}   // namespace mstd

#endif   // __MSTD_UNITS_STRINGS_HPP__