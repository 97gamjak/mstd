#ifndef __MSTD__TYPES__TAG_HPP_
#define __MSTD__TYPES__TAG_HPP_

#include <cstddef>
#include <string>
#include <string_view>

namespace mstd
{
    /**
     * @brief
     *
     * @tparam N
     */
    template <std::size_t N>
    class Tag
    {
        char value[N]{};

        constexpr Tag(const char (&str)[N]);

        [[nodiscard]]
        constexpr std::string_view view() const;

        [[nodiscard]]
        std::string to_string() const;
    };
}   // namespace mstd

#ifndef __MSTD__TYPES__TAG_TPP__
#include "tag.tpp"
#endif

#endif   // __MSTD__TYPES__TAG_HPP_
