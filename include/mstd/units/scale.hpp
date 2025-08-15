#ifndef __MSTD_UNITS_SCALE_HPP__
#define __MSTD_UNITS_SCALE_HPP__

#include "scale_tags.hpp"

namespace mstd
{
    template <
        typename Ls   = L<std::ratio<1>>,
        typename Ms   = M<std::ratio<1>>,
        typename Ts   = T<std::ratio<1>>,
        typename Ks   = K<std::ratio<1>>,
        typename Ns   = N<std::ratio<1>>,
        typename Is   = I<std::ratio<1>>,
        typename Cds  = Cd<std::ratio<1>>,
        typename Kind = void>
    struct ScalePack
    {
        using L    = Ls;
        using M    = Ms;
        using T    = Ts;
        using K    = Ks;
        using N    = Ns;
        using I    = Is;
        using Cd   = Cds;
        using kind = Kind;
    };

}   // namespace mstd

#endif   // __MSTD_UNITS_SCALE_HPP__