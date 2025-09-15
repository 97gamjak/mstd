#ifndef __MSTD_MATH_POWER_HPP__
#define __MSTD_MATH_POWER_HPP__

namespace mstd::math
{

    template <typename T>
    T power(T base, int exponent)
    {
        T result = static_cast<T>(1);

        if (exponent < 0)
        {
            base     = 1 / base;
            exponent = -exponent;
        }

        for (int i = 0; i < exponent; i++)
            result *= base;

        return result;
    }

}   // namespace mstd::math

#endif   // __MSTD_MATH_POWER_HPP__