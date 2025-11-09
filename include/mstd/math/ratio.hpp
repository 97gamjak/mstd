#ifndef __MSTD__MATH___RATIO_HPP__
#define __MSTD__MATH___RATIO_HPP__

namespace mstd
{
    template <typename T>
    struct ratio_v_impl
    {
        MSTD_COMPILE_FAIL("Unsupported type for ratio_v");
    };

    template <typename R>
    static constexpr long double ratio_v = ratio_v_impl<R>::value;

}   // namespace mstd

#endif   // __MSTD__MATH___RATIO_HPP__