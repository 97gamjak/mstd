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

#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

#include "mstd/exceptions.hpp"
#include "mstd/types/cache.hpp"
#include "mstd/types/property.hpp"

namespace
{
    // Domains are plain tags: one global epoch counter per tag.
    struct Geometry
    {
    };
    struct Kinetics
    {
    };

    using Length = mstd::CacheProperty<double, Geometry>;
    using Speed  = mstd::CacheProperty<double, Kinetics>;

    static_assert(mstd::Source<Length>);
    static_assert(mstd::Source<Speed>);
    static_assert(!mstd::Source<double>);

    // compute functions are plain function pointers, so they count calls
    // through a global.
    int calls = 0;

    double twice(const double& x)
    {
        ++calls;
        return 2.0 * x;
    }

    double product(const double& x, const double& y)
    {
        ++calls;
        return x * y;
    }

    double sum3(const double& x, const double& y, const double& z)
    {
        ++calls;
        return x + y + z;
    }

    double throwIfNegative(const double& x)
    {
        ++calls;
        if (x < 0.0)
            throw std::runtime_error("negative");
        return x;
    }
}   // namespace

TEST_CASE("Cache computes lazily and only once", "[types][cache]")
{
    calls = 0;
    Length                      a{1.0};
    mstd::Cache<double, Length> cache{twice};

    REQUIRE(calls == 0);   // nothing computed on construction

    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(calls == 1);

    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(calls == 1);   // hits do not recompute
}

TEST_CASE("Cache recomputes after its source changed", "[types][cache]")
{
    calls = 0;
    Length                      a{1.0};
    mstd::Cache<double, Length> cache{twice};

    REQUIRE(cache.get(a) == 2.0);

    a.set(5.0);
    REQUIRE(cache.get(a) == 10.0);
    REQUIRE(calls == 2);

    a.set(7.0);
    a.set(8.0);   // two writes, still a single recompute on next read
    REQUIRE(cache.get(a) == 16.0);
    REQUIRE(calls == 3);
}

TEST_CASE("Any write in the same domain invalidates (coarse)", "[types][cache]")
{
    calls = 0;
    Length                      a{1.0};
    Length                      other{3.0};
    mstd::Cache<double, Length> cache{twice};

    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(calls == 1);

    other.set(4.0);   // unrelated property, same domain
    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(calls == 2);   // recomputed once, value unchanged

    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(calls == 2);   // and valid again
}

TEST_CASE("A write in another domain does not invalidate", "[types][cache]")
{
    calls = 0;
    Length                      a{1.0};
    Speed                       v{3.0};
    mstd::Cache<double, Length> cache{twice};

    REQUIRE(cache.get(a) == 2.0);

    v.set(9.0);
    REQUIRE(cache.get(a) == 2.0);
    REQUIRE(calls == 1);
}

TEST_CASE("A cache over two domains reacts to either", "[types][cache]")
{
    calls = 0;
    Length                             a{2.0};
    Speed                              v{3.0};
    mstd::Cache<double, Length, Speed> cache{product};

    REQUIRE(cache.get(a, v) == 6.0);
    REQUIRE(cache.get(a, v) == 6.0);
    REQUIRE(calls == 1);

    a.set(5.0);
    REQUIRE(cache.get(a, v) == 15.0);
    REQUIRE(calls == 2);

    v.set(2.0);
    REQUIRE(cache.get(a, v) == 10.0);
    REQUIRE(calls == 3);

    REQUIRE(cache.get(a, v) == 10.0);
    REQUIRE(calls == 3);
}

TEST_CASE("Several sources in one domain", "[types][cache]")
{
    calls = 0;
    Length                                     a{1.0};
    Length                                     b{2.0};
    Speed                                      v{3.0};
    mstd::Cache<double, Length, Length, Speed> cache{sum3};

    REQUIRE(cache.get(a, b, v) == 6.0);
    REQUIRE(cache.get(a, b, v) == 6.0);
    REQUIRE(calls == 1);

    b.set(10.0);
    REQUIRE(cache.get(a, b, v) == 14.0);
    REQUIRE(calls == 2);

    v.set(0.0);
    REQUIRE(cache.get(a, b, v) == 11.0);
    REQUIRE(calls == 3);
}

TEST_CASE("Copy assignment of a property invalidates", "[types][cache]")
{
    calls = 0;
    Length                      a{1.0};
    Length                      b{4.0};
    mstd::Cache<double, Length> cache{twice};

    REQUIRE(cache.get(a) == 2.0);

    a = b;
    REQUIRE(cache.get(a) == 8.0);
    REQUIRE(calls == 2);
}

TEST_CASE("A throwing compute keeps the cache invalid", "[types][cache]")
{
    calls = 0;
    Length                      a{1.0};
    mstd::Cache<double, Length> cache{throwIfNegative};

    REQUIRE(cache.get(a) == 1.0);

    a.set(-1.0);
    REQUIRE_THROWS_AS(cache.get(a), std::runtime_error);
    REQUIRE_THROWS_AS(cache.get(a), std::runtime_error);   // not served stale
    REQUIRE(calls == 3);

    a.set(2.0);
    REQUIRE(cache.get(a) == 2.0);
}

TEST_CASE("A cache without compute function throws", "[types][cache]")
{
    Length                      a{1.0};
    mstd::Cache<double, Length> cache{nullptr};

    REQUIRE_THROWS_AS(cache.get(a), mstd::RuntimeError);
}

TEST_CASE("CacheProperty versions", "[types][property]")
{
    Length a{1.0};
    Length b{1.0};

    // every property gets a fresh version
    REQUIRE(a.version() != b.version());

    const auto before = a.version();
    a.set(2.0);
    REQUIRE(a.version() > before);
    REQUIRE(a.get() == 2.0);

    Length copy{a};
    REQUIRE(copy.get() == 2.0);
    REQUIRE(copy.version() != a.version());
}
