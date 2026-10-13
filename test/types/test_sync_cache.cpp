/*****************************************************************************
<GPL_HEADER>

    mstd library
    Copyright (C) 2025-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <latch>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "mstd/exceptions.hpp"
#include "mstd/types/cache/member_cache.hpp"
#include "mstd/types/cache/sync_cache.hpp"
#include "mstd/types/property.hpp"

namespace
{
    struct Geometry
    {
    };
    struct Kinetics
    {
    };

    using Length = mstd::CacheProperty<double, Geometry>;
    using Speed  = mstd::CacheProperty<double, Kinetics>;

    std::atomic<int> areaCalls{0};
    std::atomic<int> plusCalls{0};
    std::atomic<int> scaledCalls{0};

    double areaFn(const double& x, const double& y)
    {
        ++areaCalls;
        // make a recompute slow enough for threads to pile up
        std::this_thread::sleep_for(std::chrono::microseconds(200));
        return x * y;
    }

    double plusFn(const double& m)
    {
        ++plusCalls;
        return m + 1.0;
    }

    double scaledFn(const double& m, const double& v)
    {
        ++scaledCalls;
        return m * v;
    }

    void resetCalls()
    {
        areaCalls   = 0;
        plusCalls   = 0;
        scaledCalls = 0;
    }

    // An owner mixing both cache kinds; copy, move and assignment are all
    // defaulted.
    struct Shape
    {
        Length a{2.0};
        Length b{3.0};
        Speed  v{10.0};

        mstd::SyncCache<double, Shape, &Shape::a, &Shape::b> area{areaFn};

        // a SyncCache as source of a SyncCache
        mstd::SyncCache<double, Shape, &Shape::area> plus{plusFn};

        // a SyncCache as source of a MemberCache
        mstd::MemberCache<double, Shape, &Shape::area, &Shape::v> scaled{
            scaledFn
        };

        // a SyncCache of a SyncCache and a property, for the threaded tests
        mstd::SyncCache<double, Shape, &Shape::area, &Shape::v> scaledSync{
            scaledFn
        };

        [[nodiscard]] double getArea() const { return area.get(*this); }
        [[nodiscard]] double getScaledSync() const
        {
            return scaledSync.get(*this);
        }
        [[nodiscard]] double getPlus() const { return plus.get(*this); }
        [[nodiscard]] double getScaled() const { return scaled.get(*this); }
    };

    static_assert(std::is_same_v<
                  decltype(Shape::area)::domains,
                  mstd::detail::DomainList<Geometry>>);
    static_assert(std::is_same_v<
                  decltype(Shape::plus)::domains,
                  mstd::detail::DomainList<Geometry>>);

    static_assert(std::is_copy_constructible_v<Shape>);
    static_assert(std::is_copy_assignable_v<Shape>);
    static_assert(std::is_move_constructible_v<Shape>);
    static_assert(std::is_move_assignable_v<Shape>);

    double throwIfNegative(const double& x)
    {
        if (x < 0.0)
            throw std::runtime_error("negative");
        return x;
    }

    struct Fragile
    {
        Length                                        x{1.0};
        mstd::SyncCache<double, Fragile, &Fragile::x> value{throwIfNegative};
    };

    // run `work(threadIndex)` on `count` threads that start together
    template <typename Work>
    void runTogether(int count, Work work)
    {
        std::latch               start(count);
        std::vector<std::thread> threads;
        threads.reserve(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i)
        {
            threads.emplace_back(
                [&, i]
                {
                    start.arrive_and_wait();
                    work(i);
                }
            );
        }
        for (auto& thread : threads)
            thread.join();
    }
}   // namespace

TEST_CASE("SyncCache computes lazily and only once", "[types][sync_cache]")
{
    resetCalls();
    Shape s;

    REQUIRE(areaCalls == 0);

    REQUIRE(s.getArea() == 6.0);
    REQUIRE(s.getArea() == 6.0);
    REQUIRE(areaCalls == 1);
}

TEST_CASE("SyncCache recomputes after a source changed", "[types][sync_cache]")
{
    resetCalls();
    Shape s;
    REQUIRE(s.getArea() == 6.0);

    s.a.set(5.0);
    REQUIRE(s.getArea() == 15.0);
    REQUIRE(areaCalls == 2);

    s.v.set(1.0);   // other domain: no recompute
    REQUIRE(s.getArea() == 15.0);
    REQUIRE(areaCalls == 2);
}

TEST_CASE("Caches of both kinds can be chained", "[types][sync_cache]")
{
    resetCalls();
    Shape s;

    REQUIRE(s.getPlus() == 7.0);
    REQUIRE(s.getScaled() == 60.0);
    REQUIRE(areaCalls == 1);   // shared source computed once
    REQUIRE(plusCalls == 1);
    REQUIRE(scaledCalls == 1);

    s.v.set(2.0);   // only the MemberCache is affected
    REQUIRE(s.getScaled() == 12.0);
    REQUIRE(s.getPlus() == 7.0);
    REQUIRE(areaCalls == 1);
    REQUIRE(plusCalls == 1);
    REQUIRE(scaledCalls == 2);

    s.b.set(5.0);   // everything is affected
    REQUIRE(s.getPlus() == 11.0);
    REQUIRE(s.getScaled() == 20.0);
    REQUIRE(areaCalls == 2);
}

TEST_CASE(
    "An owner keeps defaulted copy, move and assignment",
    "[types][sync_cache]"
)
{
    Shape s;
    REQUIRE(s.getPlus() == 7.0);

    Shape copy{s};
    REQUIRE(copy.getPlus() == 7.0);

    copy.a.set(10.0);
    REQUIRE(copy.getPlus() == 31.0);
    REQUIRE(s.getPlus() == 7.0);

    s = copy;
    REQUIRE(s.getPlus() == 31.0);
    s.b.set(1.0);
    REQUIRE(s.getArea() == 10.0);
    REQUIRE(copy.getArea() == 30.0);

    Shape moved{std::move(s)};
    REQUIRE(moved.getArea() == 10.0);
}

TEST_CASE(
    "Concurrent readers of a stale cache compute exactly once",
    "[types][sync_cache][threads]"
)
{
    // many rounds, each with a freshly stale cache; the former
    // implementation threw "no compute function" for the losers of the race
    for (int round = 0; round < 100; ++round)
    {
        resetCalls();
        Shape s;
        s.a.set(1.0 + round);   // written before the threads start

        constexpr int       threads = 8;
        std::vector<double> results(threads, 0.0);

        runTogether(
            threads,
            [&](int i) { results[static_cast<std::size_t>(i)] = s.getArea(); }
        );

        REQUIRE(areaCalls == 1);
        for (const double r : results)
            REQUIRE(r == (1.0 + round) * 3.0);
    }
}

TEST_CASE("Concurrent readers of a chain", "[types][sync_cache][threads]")
{
    for (int round = 0; round < 50; ++round)
    {
        resetCalls();
        Shape s;
        s.b.set(2.0 + round);

        constexpr int     threads = 8;
        std::atomic<bool> ok{true};

        runTogether(
            threads,
            [&](int i)
            {
                const double area = 2.0 * (2.0 + round);
                if (i % 2 == 0)
                {
                    if (s.getPlus() != area + 1.0)
                        ok = false;
                }
                else
                {
                    if (s.getScaledSync() != area * 10.0)
                        ok = false;
                }
            }
        );

        REQUIRE(ok);
        REQUIRE(areaCalls == 1);
        REQUIRE(plusCalls == 1);
        REQUIRE(scaledCalls == 1);
    }
}

TEST_CASE(
    "Readers see the value after a serial write",
    "[types][sync_cache][threads]"
)
{
    resetCalls();
    Shape             s;
    constexpr int     threads = 4;
    std::atomic<bool> ok{true};

    for (int round = 1; round <= 20; ++round)
    {
        s.a.set(static_cast<double>(round));   // serial section

        // Catch2 assertions are not thread-safe: collect, check afterwards
        runTogether(
            threads,
            [&](int)
            {
                if (s.getArea() != static_cast<double>(round) * 3.0)
                    ok = false;
            }
        );
    }
    REQUIRE(ok);
    REQUIRE(areaCalls == 20);
}

TEST_CASE("A throwing compute is retried, not cached", "[types][sync_cache]")
{
    Fragile f;
    REQUIRE(f.value.get(f) == 1.0);

    f.x.set(-1.0);
    REQUIRE_THROWS_AS(f.value.get(f), std::runtime_error);
    REQUIRE_THROWS_AS(f.value.get(f), std::runtime_error);

    f.x.set(2.0);
    REQUIRE(f.value.get(f) == 2.0);
}

TEST_CASE("A SyncCache without compute function throws", "[types][sync_cache]")
{
    struct Holder
    {
        Length                                      x{1.0};
        mstd::SyncCache<double, Holder, &Holder::x> value{nullptr};
    };

    Holder h;
    REQUIRE_THROWS_AS(h.value.get(h), mstd::RuntimeError);
}
