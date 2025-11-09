#ifndef __MSTD__PACK__PACK_HPP__
#define __MSTD__PACK__PACK_HPP__

namespace mstd
{
    template <
        std::size_t I,
        template <class...> typename F,
        typename Pack0,
        typename... Packs>
    using zip_at_t =
        F<typename Pack0::template type_at<I>,
          typename Packs::template type_at<I>...>;

    template <typename Pack, typename... Us>
    struct pack_rebind;

    template <template <typename...> typename P, typename... Ts, typename... Us>
    struct pack_rebind<P<Ts...>, Us...>
    {
        using type = P<Us...>;
    };

    template <typename Pack, typename... Us>
    using pack_rebind_t = typename pack_rebind<Pack, Us...>::type;

}   // namespace mstd

#endif   // __MSTD__PACK__PACK_HPP__