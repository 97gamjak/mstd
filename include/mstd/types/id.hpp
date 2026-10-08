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
    };

}   // namespace mstd

#endif   // __MSTD__TYPES__ID_HPP__
