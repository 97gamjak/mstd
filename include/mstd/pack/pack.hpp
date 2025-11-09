#ifndef __MSTD__PACK__PACK_HPP__
#define __MSTD__PACK__PACK_HPP__

namespace mstd
{
    template <
        std::size_t I,
        template <class...> class F,
        class Pack0,
        class... Packs>
    using zip_at_t =
        F<typename Pack0::template type_at<I>,
          typename Packs::template type_at<I>...>;

    template <class Pack, class... Us>
    struct pack_rebind;

    template <template <class...> class P, class... Ts, class... Us>
    struct pack_rebind<P<Ts...>, Us...>
    {
        using type = P<Us...>;
    };

    template <class Pack, class... Us>
    using pack_rebind_t = typename pack_rebind<Pack, Us...>::type;

}   // namespace mstd

#endif   // __MSTD__PACK__PACK_HPP__