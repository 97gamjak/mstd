/*****************************************************************************
<GPL_HEADER>

    mstd library
    Copyright (C) 2025-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#ifndef __MSTD__TYPES__STRONG_TYPE_HPP__
#define __MSTD__TYPES__STRONG_TYPE_HPP__

#include <compare>
#include <concepts>
#include <ostream>

#include "mstd/enum.hpp"
#include "mstd/type_traits/string.hpp"

namespace mstd
{

#define STRONG_TYPE_TRAIT_LIST(X) \
    X(NONE, 0)                    \
    X(ORDERED, 1u << 0)           \
    X(ARITHMETIC, 1u << 1)        \
    X(INCREMENT, 1u << 2)         \
    X(HASHABLE, 1u << 3)          \
    X(STREAMABLE, 1u << 4)        \
    X(BOOLEAN, 1u << 5)

    MSTD_ENUM_BITFLAG(StrongTypeTrait, unsigned, STRONG_TYPE_TRAIT_LIST);

    /**
     * @brief A generalized strong type wrapper.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * Usage:
     *   struct EnergyTag{};
     *   using Energy = StrongType<double, EnergyTag, StrongTypeTrait::ORDERED |
     *                                                 StrongTypeTrait::ARITHMETIC
     * | StrongTypeTrait::STREAMABLE>;
     */
    template <
        typename T,
        typename Tag,
        StrongTypeTrait Traits = StrongTypeTrait::NONE>
    class StrongType
    {
       private:
        T _value{};

       public:
        using ValueType                  = T;
        using TagType                    = Tag;
        static constexpr auto traitFlags = Traits;

        constexpr StrongType() = default;

        constexpr explicit StrongType(
            const T &value
        ) noexcept(std::is_nothrow_copy_constructible_v<T>);

        constexpr explicit StrongType(
            T &&value
        ) noexcept(std::is_nothrow_move_constructible_v<T>);

        [[nodiscard]] constexpr const T &get() const noexcept;
        [[nodiscard]] constexpr T       &get() noexcept;

        [[nodiscard]] constexpr explicit operator const T &() const noexcept;

        //
        // boolean and ordering
        //

        [[nodiscard]]
        constexpr explicit operator bool() const noexcept
        requires(
            (Traits & StrongTypeTrait::BOOLEAN) && std::convertible_to<T, bool>
        );

        [[nodiscard]]
        constexpr auto operator<=>(const StrongType &) const
        requires(Traits &StrongTypeTrait::ORDERED &&
                 std::three_way_comparable<T>)
        = default;

        [[nodiscard]]
        constexpr bool operator==(const StrongType &) const
        requires(Traits &StrongTypeTrait::ORDERED &&
                 std::equality_comparable<T>)
        = default;

        //
        // Arithmetic
        //

        constexpr StrongType &operator+=(const StrongType &other)
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        constexpr StrongType &operator-=(const StrongType &other)
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        constexpr StrongType &operator*=(const T &scalar)
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        constexpr StrongType &operator/=(const T &scalar)
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        [[nodiscard]]
        constexpr StrongType operator+(const StrongType &other) const
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        [[nodiscard]]
        constexpr StrongType operator-(const StrongType &other) const
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        [[nodiscard]]
        constexpr StrongType operator*(const T &scalar) const
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        [[nodiscard]]
        constexpr StrongType operator/(const T &scalar) const
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        [[nodiscard]]
        constexpr StrongType operator-() const
        requires(bool(Traits &StrongTypeTrait::ARITHMETIC));

        //
        // Increment / decrement
        //

        constexpr StrongType &operator++()
        requires(bool(Traits &StrongTypeTrait::INCREMENT));

        constexpr StrongType operator++(int)
        requires(bool(Traits &StrongTypeTrait::INCREMENT));

        constexpr StrongType &operator--()
        requires(bool(Traits &StrongTypeTrait::INCREMENT));

        constexpr StrongType operator--(int)
        requires(bool(Traits &StrongTypeTrait::INCREMENT));

        std::string toString() const
        requires HasToString<Tag, T>;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__STRONG_TYPE_TPP__
#include "strong_type.tpp"   // IWYU pragma: export
#endif                       // __MSTD__TYPES__STRONG_TYPE_TPP__

// Streaming - free function, only valid to call if STREAMABLE trait set
template <typename T, typename Tag, mstd::StrongTypeTrait Traits>
std::ostream &operator<<(
    std::ostream                           &os,
    const mstd::StrongType<T, Tag, Traits> &strongType
)
requires(Traits &mstd::StrongTypeTrait::STREAMABLE && true)
{
    return os << strongType.get();
}

#endif   // __MSTD__TYPES__STRONG_TYPE_HPP__
