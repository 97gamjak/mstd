#ifndef __MSTD__TYPES__ID_HPP__
#define __MSTD__TYPES__ID_HPP__

namespace mstd
{

    struct DefaultTag
    {
    };

    /**
     * @brief A class representing a unique identifier of type T.
     *
     * @tparam T The type of the unique identifier.
     */
    template <typename T, typename Tag = DefaultTag>
    class Id
    {
       private:
        T _value;

       public:
        Id() = default;
        explicit Id(T v);

        static Id next();

        constexpr bool operator==(const Id& other) const noexcept = default;
    };

}   // namespace mstd

#ifndef __MSTD__TYPES__ID_TPP__
#include "id.tpp"
#endif

#endif   // __MSTD__TYPES__ID_HPP__
