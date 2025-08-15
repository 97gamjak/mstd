#ifndef __MSTD_UNITS_DIMENSION_ALIASES_HPP__
#define __MSTD_UNITS_DIMENSION_ALIASES_HPP__

#include "dimension.hpp"

namespace mstd
{
    using D_Length   = dimension<1, 0, 0, 0, 0, 0, 0>;
    using D_Mass     = dimension<0, 1, 0, 0, 0, 0, 0>;
    using D_Time     = dimension<0, 0, 1, 0, 0, 0, 0>;
    using D_Temp     = dimension<0, 0, 0, 1, 0, 0, 0>;
    using D_Amount   = dimension<0, 0, 0, 0, 1, 0, 0>;
    using D_I        = dimension<0, 0, 0, 0, 0, 1, 0>;
    using D_Cd       = dimension<0, 0, 0, 0, 0, 0, 1>;
    using D_UnitLess = dimension<0, 0, 0, 0, 0, 0, 0>;

    using D_Velocity     = Dimension_sub<D_Length, D_Time>;
    using D_Acceleration = Dimension_sub<D_Velocity, D_Time>;

}   // namespace mstd

#endif   // __MSTD_UNITS_DIMENSION_ALIASES_HPP__