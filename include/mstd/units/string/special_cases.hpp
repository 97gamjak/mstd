#ifndef __MSTD_UNITS_STRING_SPECIAL_CASES_HPP__
#define __MSTD_UNITS_STRING_SPECIAL_CASES_HPP__

#include <optional>
#include <ratio>
#include <string>

#include "../base.hpp"
#include "../scale_tags.hpp"

namespace mstd
{
    namespace units::strings
    {

        template <typename Ratio>
        constexpr void get_special_mass_cases(
            std::optional<std::string>& prefix,
            std::optional<std::string>& base_unit,
            std::optional<std::string>& exponent_str
        )
        {
            prefix    = get_prefix<std::ratio_multiply<Ratio, std::kilo>>();
            base_unit = "g";
        }

        template <typename Ratio>
        constexpr void get_special_time_cases(
            std::optional<std::string>& prefix,
            std::optional<std::string>& base_unit,
            std::optional<std::string>& exponent_str
        )
        {
            if (std::same_as<Ratio, min::ratio>)
            {
                prefix    = "";
                base_unit = "min";
                return;
            }
            else if (std::same_as<Ratio, h::ratio>)
            {
                prefix    = "";
                base_unit = "h";
                return;
            }
            else if (std::same_as<Ratio, yr::ratio>)
            {
                prefix    = "";
                base_unit = "yr";
                return;
            }

            // catch special case that for seconds higher than 1
            // there should not exist any kind of ks or similar
            if (Ratio::num / Ratio::den > 1)
            {
                prefix = "";
                return;
            }
        }

        template <typename Dim, typename Ratio>
        constexpr void get_special_length_cases(
            std::optional<std::string>& prefix,
            std::optional<std::string>& base_unit,
            std::optional<std::string>& exponent_str
        )
        {
            if (Dim::exponents[Dim::length_index()] == 1 &&
                std::same_as<Ratio, Ang::ratio>)
            {
                prefix    = "";
                base_unit = "Ang";
                return;
            }

            if (Dim::exponents[Dim::length_index()] == 3 &&
                std::same_as<Ratio, dm::ratio>)
            {
                prefix       = "";
                base_unit    = "l";
                exponent_str = "";
                return;
            }
        }

        template <typename Dim, typename Ratio, BaseUnitEnum enumV>
        constexpr void get_special_unit_cases(
            std::optional<std::string>& prefix,
            std::optional<std::string>& base_unit,
            std::optional<std::string>& exponent_str
        )
        {
            if constexpr (enumV == BaseUnitEnum::Length)
                get_special_length_cases<Dim, Ratio>(
                    prefix,
                    base_unit,
                    exponent_str
                );
            else if constexpr (enumV == BaseUnitEnum::Mass)
                get_special_mass_cases<Ratio>(prefix, base_unit, exponent_str);
            else if constexpr (enumV == BaseUnitEnum::Time)
                get_special_time_cases<Ratio>(prefix, base_unit, exponent_str);
        }

    }   // namespace units::strings

}   // namespace mstd

#endif   // __MSTD_UNITS_STRING_SPECIAL_CASES_HPP__