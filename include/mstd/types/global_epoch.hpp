#ifndef __MSTD__TYPES__GLOBAL_EPOCH_HPP__
#define __MSTD__TYPES__GLOBAL_EPOCH_HPP__

#include <atomic>
#include <cstdint>

namespace mstd
{
    using EpochValueType = std::uint64_t;

    /**
     * @brief A global epoch counter for a specific domain.
     *
     * This class provides a static atomic counter that can be used to track
     * the global epoch for a given domain. It supports querying the current
     * epoch and advancing it.
     */
    template <typename Domain>
    struct GlobalEpoch
    {
        alignas(64) static inline std::atomic<EpochValueType> value{0};

        /**
         * @brief Get the current global epoch.
         *
         * @return The current value of the global epoch counter.
         */
        [[nodiscard]]
        static EpochValueType now() noexcept
        {
            return value.load(std::memory_order_relaxed);
        }

        /**
         * @brief Advance the global epoch by one.
         *
         * @return The new value of the global epoch counter after incrementing.
         */
        [[nodiscard]]
        static EpochValueType advance() noexcept
        {
            return value.fetch_add(1, std::memory_order_relaxed) + 1;
        }
    };
}   // namespace mstd

#endif   // __MSTD__TYPES__GLOBAL_EPOCH_HPP__
