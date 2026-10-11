#ifndef __MSTD__TYPES__GLOBAL_EPOCH_HPP__
#define __MSTD__TYPES__GLOBAL_EPOCH_HPP__

#include <atomic>
#include <cstdint>

namespace mstd
{
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
        using Underlying = std::uint64_t;

        alignas(64) static inline std::atomic<Underlying> value{0};

        /**
         * @brief Get the current global epoch.
         *
         * @return The current value of the global epoch counter.
         */
        static Underlying now() noexcept
        {
            return value.load(std::memory_order_relaxed);
        }

        /**
         * @brief Advance the global epoch by one.
         *
         * @return The new value of the global epoch counter after incrementing.
         */
        static Underlying advance() noexcept
        {
            return value.fetch_add(1, std::memory_order_relaxed) + 1;
        }
    };
}   // namespace mstd

#endif   // __MSTD__TYPES__GLOBAL_EPOCH_HPP__
