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

#ifndef __MSTD__TYPES__REF_HPP__
#define __MSTD__TYPES__REF_HPP__

#include <concepts>
#include <functional>
#include <type_traits>

namespace mstd
{
    /**
     * @brief A rebindable, non-null reference wrapper around
     * std::reference_wrapper with pointer-like access.
     *
     * Intended as a replacement for reference (or const) data members, which
     * delete copy/move assignment (see C++ Core Guidelines C.12). In contrast
     * to std::reference_wrapper it
     *  - provides operator-> and operator* (no `.get().member` noise),
     *  - can never be constructed from an rvalue (no dangling temporaries),
     *  - converts implicitly from Ref<U> to Ref<T> where U& converts to T&
     *    (derived to base, non-const to const).
     *
     * The wrapped object must outlive the Ref. Ref never owns.
     *
     * @tparam T referenced type, may be const-qualified
     *
     * Usage:
     *   class Thermostat
     *   {
     *       mstd::ConstRef<ThermostatSettings> _settings;
     *
     *      public:
     *       explicit Thermostat(mstd::ConstRef<ThermostatSettings> settings)
     *           : _settings(settings) {}
     *
     *       double target() const { return _settings->temperature; }
     *   };
     */
    template <typename T>
    class Ref
    {
       private:
        std::reference_wrapper<T> _ref;

       public:
        using type = T;

        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(google-explicit-constructor)
        constexpr Ref(T &ref) noexcept;
        Ref(T &&) = delete;   // forbid binding to rvalues

        template <typename U>
        requires(!std::same_as<U, T> && std::is_convertible_v<U &, T &>)
        // cppcheck-suppress noExplicitConstructor
        constexpr Ref(
            const Ref<U> &other   // NOLINT(google-explicit-constructor)
        ) noexcept;

        [[nodiscard]] constexpr T &get() const noexcept;

        [[nodiscard]] constexpr T &operator*() const noexcept;
        [[nodiscard]] constexpr T *operator->() const noexcept;

        // NOLINTNEXTLINE(google-explicit-constructor)
        [[nodiscard]] constexpr operator T &() const noexcept;
    };

    template <typename T>
    Ref(T &) -> Ref<T>;

    /// shorthand for a reference to const, the common case for settings
    template <typename T>
    using ConstRef = Ref<const T>;

}   // namespace mstd

#ifndef __MSTD__TYPES__REF_TPP__
#include "ref.tpp"   // IWYU pragma: export
#endif

#endif   // __MSTD__TYPES__REF_HPP__
