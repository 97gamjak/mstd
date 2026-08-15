#ifndef __MSTD__TYPES__STRONG_TYPE_TPP__
#define __MSTD__TYPES__STRONG_TYPE_TPP__

#include "strong_type.hpp"

namespace mstd
{
    /**
     * @brief StrongType constructor from const reference.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param value the underlying value to wrap
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits>::StrongType(
        const T &value
    ) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : _value(value)
    {
    }

    /**
     * @brief StrongType constructor from rvalue reference.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param value the underlying value to wrap
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits>::StrongType(
        T &&value
    ) noexcept(std::is_nothrow_move_constructible_v<T>)
        : _value(std::move(value))
    {
    }

    /**
     * @brief Get the underlying value.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return const T& reference to the underlying value
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr const T &StrongType<T, Tag, Traits>::get() const noexcept
    {
        return _value;
    }

    /**
     * @brief Get the underlying value.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return T& reference to the underlying value
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr T &StrongType<T, Tag, Traits>::get() noexcept
    {
        return _value;
    }

    /**
     * @brief Explicit conversion operator to const T&.
     *
     * This operator allows explicit conversion from StrongType to the
     * underlying type T. It is only enabled if the
     * STRONG_TYPE_TRAIT::EXPLICIT_CONVERSION trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return const T& reference to the underlying value
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits>::operator const T &() const noexcept
    {
        return _value;
    }

    /**
     * @brief Explicit conversion operator to bool.
     *
     * This operator allows explicit conversion from StrongType to bool. It is
     * only enabled if the STRONG_TYPE_TRAIT::BOOLEAN trait is set and T is
     * convertible to bool.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return true if the underlying value is truthy, false otherwise
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits>::operator bool() const noexcept
    requires(
        (Traits & StrongTypeTrait::BOOLEAN) && std::convertible_to<T, bool>
    )
    {
        return static_cast<bool>(_value);
    }

    /**
     * @brief Compound assignment operator for addition.
     *
     * This operator allows adding another StrongType of the same type to this
     * StrongType. It is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait
     * is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param other the other StrongType to add
     * @return reference to this StrongType after addition
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> &StrongType<T, Tag, Traits>::
    operator+=(const StrongType &other)
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        _value += other._value;
        return *this;
    }

    /**
     * @brief Compound assignment operator for subtraction.
     *
     * This operator allows subtracting another StrongType of the same type from
     * this StrongType. It is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC
     * trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param other the other StrongType to subtract
     * @return reference to this StrongType after subtraction
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> &StrongType<T, Tag, Traits>::
    operator-=(const StrongType &other)
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        _value -= other._value;
        return *this;
    }

    /**
     * @brief Compound assignment operator for multiplication by a scalar.
     *
     * This operator allows multiplying this StrongType by a scalar of type T.
     * It is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param scalar the scalar value to multiply by
     * @return reference to this StrongType after multiplication
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> &StrongType<T, Tag, Traits>::
    operator*=(const T &scalar)
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        _value *= scalar;
        return *this;
    }

    /**
     * @brief Compound assignment operator for division by a scalar.
     *
     * This operator allows dividing this StrongType by a scalar of type T. It
     * is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param scalar the scalar value to divide by
     * @return reference to this StrongType after division
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> &StrongType<T, Tag, Traits>::
    operator/=(const T &scalar)
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        _value /= scalar;
        return *this;
    }

    /**
     * @brief Addition operator for StrongType.
     *
     * This operator allows adding two StrongType objects of the same type. It
     * is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param other the other StrongType to add
     * @return a new StrongType representing the sum
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator+(
        const StrongType &other
    ) const
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        return StrongType{_value + other._value};
    }

    /**
     * @brief Subtraction operator for StrongType.
     *
     * This operator allows subtracting one StrongType from another of the same
     * type. It is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait is
     * set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param other the other StrongType to subtract
     * @return a new StrongType representing the difference
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator-(
        const StrongType &other
    ) const
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        return StrongType{_value - other._value};
    }

    /**
     * @brief Multiplication operator for StrongType by a scalar.
     *
     * This operator allows multiplying a StrongType by a scalar of type T. It
     * is only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param scalar the scalar value to multiply by
     * @return a new StrongType representing the product
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator*(
        const T &scalar
    ) const
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        return StrongType{_value * scalar};
    }

    /**
     * @brief Division operator for StrongType by a scalar.
     *
     * This operator allows dividing a StrongType by a scalar of type T. It is
     * only enabled if the STRONG_TYPE_TRAIT::ARITHMETIC trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @param scalar the scalar value to divide by
     * @return a new StrongType representing the quotient
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator/(
        const T &scalar
    ) const
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        return StrongType{_value / scalar};
    }

    /**
     * @brief Unary negation operator for StrongType.
     *
     * This operator allows negating a StrongType. It is only enabled if the
     * STRONG_TYPE_TRAIT::ARITHMETIC trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return a new StrongType representing the negated value
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator-(
    ) const
    requires(bool(Traits &StrongTypeTrait::ARITHMETIC))
    {
        return StrongType{-_value};
    }

    /**
     * @brief Pre-increment operator for StrongType.
     *
     * This operator allows pre-incrementing a StrongType. It is only enabled if
     * the STRONG_TYPE_TRAIT::INCREMENT trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return reference to this StrongType after incrementing
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> &StrongType<T, Tag, Traits>::
    operator++()
    requires(bool(Traits &StrongTypeTrait::INCREMENT))
    {
        ++_value;
        return *this;
    }

    /**
     * @brief Post-increment operator for StrongType.
     *
     * This operator allows post-incrementing a StrongType. It is only enabled
     * if the STRONG_TYPE_TRAIT::INCREMENT trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return a new StrongType representing the value before incrementing
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator++(
        int
    )
    requires(bool(Traits &StrongTypeTrait::INCREMENT))
    {
        auto tmp = *this;
        ++_value;
        return tmp;
    }

    /**
     * @brief Pre-decrement operator for StrongType.
     *
     * This operator allows pre-decrementing a StrongType. It is only enabled if
     * the STRONG_TYPE_TRAIT::INCREMENT trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return reference to this StrongType after decrementing
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> &StrongType<T, Tag, Traits>::
    operator--()
    requires(bool(Traits &StrongTypeTrait::INCREMENT))
    {
        --_value;
        return *this;
    }

    /**
     * @brief Post-decrement operator for StrongType.
     *
     * This operator allows post-decrementing a StrongType. It is only enabled
     * if the STRONG_TYPE_TRAIT::INCREMENT trait is set.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return a new StrongType representing the value before decrementing
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    constexpr StrongType<T, Tag, Traits> StrongType<T, Tag, Traits>::operator--(
        int
    )
    requires(bool(Traits &StrongTypeTrait::INCREMENT))
    {
        auto tmp = *this;
        --_value;
        return tmp;
    }

    /**
     * @brief Convert StrongType to string.
     *
     * This function converts the StrongType to a string representation. It is
     * only enabled if the Tag type provides a static toString function that
     * accepts the underlying type T.
     *
     * @tparam T underlying value type
     * @tparam Tag a unique tag type used only for disambiguation, e.g. `struct
     * EnergyTag;`
     * @tparam Traits bitwise-OR of StrongTypeTrait flags enabling specific
     * operations
     *
     * @return a string representation of the StrongType
     */
    template <typename T, typename Tag, StrongTypeTrait Traits>
    std::string StrongType<T, Tag, Traits>::toString() const
    requires HasToString<Tag, T>
    {
        return Tag::toString(_value);
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__STRONG_TYPE_TPP__
