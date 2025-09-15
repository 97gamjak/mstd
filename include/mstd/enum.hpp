#ifndef __MSTD_ENUM_HPP__
#define __MSTD_ENUM_HPP__

#include <array>         // IWYU pragma: keep
#include <cstddef>       // IWYU pragma: keep
#include <optional>      // IWYU pragma: keep
#include <string_view>   // IWYU pragma: keep
#include <type_traits>   // IWYU pragma: keep

// ------------------------------------------------------------
// Element expanders
// ------------------------------------------------------------
// X(Name)       -> Name,
// X(Name, 42)   -> Name = 42,
#define MSTD_ENUM_MAKE_ENUM(name, ...) name __VA_OPT__(= __VA_ARGS__),

// X(Name) or X(Name, 42) -> EnumName::Name,
#define MSTD_ENUM_MAKE_VALUE(name, ...) name,

// X(Name) -> "Name",
#define MSTD_ENUM_MAKE_STRING(name, ...) #name,

// ------------------------------------------------------------
// Main macro
// ------------------------------------------------------------
#define MSTD_ENUM(EnumName, Underlying, LIST)                               \
    enum class EnumName : Underlying                                        \
    {                                                                       \
        LIST(MSTD_ENUM_MAKE_ENUM)                                           \
    };                                                                      \
                                                                            \
    struct EnumName##Meta                                                   \
    {                                                                       \
        using type            = EnumName;                                   \
        using underlying_type = Underlying;                                 \
        using enum EnumName;                                                \
                                                                            \
        static constexpr auto values =                                      \
            std::to_array<EnumName>({LIST(MSTD_ENUM_MAKE_VALUE)});          \
                                                                            \
        static constexpr auto names =                                       \
            std::to_array<std::string_view>({LIST(MSTD_ENUM_MAKE_STRING)}); \
                                                                            \
        static constexpr std::size_t size = values.size();                  \
                                                                            \
        static constexpr auto begin() { return values.begin(); }            \
        static constexpr auto end() { return values.end(); }                \
                                                                            \
        static constexpr std::string_view name(EnumName e)                  \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (values[i] == e)                                         \
                    return names[i];                                        \
            return {};                                                      \
        }                                                                   \
                                                                            \
        static constexpr std::optional<EnumName> from_string(               \
            std::string_view s                                              \
        )                                                                   \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (names[i] == s)                                          \
                    return values[i];                                       \
            return std::nullopt;                                            \
        }                                                                   \
                                                                            \
        static constexpr underlying_type to_underlying(EnumName e)          \
        {                                                                   \
            return static_cast<underlying_type>(e);                         \
        }                                                                   \
                                                                            \
        static constexpr std::size_t index(EnumName e)                      \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (values[i] == e)                                         \
                    return i;                                               \
            return static_cast<std::size_t>(-1);                            \
        }                                                                   \
    };

#endif   // __MSTD_ENUM_HPP__