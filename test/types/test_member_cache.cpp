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
#include <type_traits>
#include <utility>

#include "mstd/exceptions.hpp"
#include "mstd/types/cache/member_cache.hpp"
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

    int areaCalls   = 0;
    int plusCalls   = 0;
    int scaledCalls = 0;

    double areaFn(const double& x, const double& y)
    {
        ++areaCalls;
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

    // An owner with properties and caches: copy, move and assignment are
    // all defaulted.
    struct Shape
    {
        Length a{2.0};
        Length b{3.0};
        Speed  v{10.0};

        // depends on two properties of one domain
        mstd::MemberCache<double, Shape, &Shape::a, &Shape::b> area{areaFn};

        // depends on another cache
        mstd::MemberCache<double, Shape, &Shape::area> plus{plusFn};

        // depends on a cache and a property of another domain
        mstd::MemberCache<double, Shape, &Shape::area, &Shape::v> scaled{
            scaledFn
        };

        [[nodiscard]] double getArea() const { return area.get(*this); }
        [[nodiscard]] double getPlus() const { return plus.get(*this); }
        [[nodiscard]] double getScaled() const { return scaled.get(*this); }
    };

    // domains are collected and listed once
    static_assert(std::is_same_v<
                  decltype(Shape::area)::domains,
                  mstd::detail::DomainList<Geometry>>);
    static_assert(std::is_same_v<
                  decltype(Shape::plus)::domains,
                  mstd::detail::DomainList<Geometry>>);
    static_assert(std::is_same_v<
                  decltype(Shape::scaled)::domains,
                  mstd::detail::DomainList<Geometry, Kinetics>>);

    static_assert(std::is_copy_constructible_v<Shape>);
    static_assert(std::is_copy_assignable_v<Shape>);
    static_assert(std::is_move_constructible_v<Shape>);
    static_assert(std::is_move_assignable_v<Shape>);

    // sources may live in a base class
    struct Base
    {
       protected:
        Length _length{4.0};
    };

    struct Derived : Base
    {
        mstd::MemberCache<double, Derived, &Derived::_length> half{
            [](const double& x) { return x / 2.0; }
        };

        [[nodiscard]] double getHalf() const { return half.get(*this); }
        void                 setLength(double x) { _length.set(x); }
    };

    double throwIfNegative(const double& x)
    {
        if (x < 0.0)
            throw std::runtime_error("negative");
        return x;
    }

    struct Fragile
    {
        Length                                          x{1.0};
        mstd::MemberCache<double, Fragile, &Fragile::x> value{throwIfNegative};
    };

    // a box like owner: matrix once, the inverse only on demand and a
    // volume taken from the matrix
    int matrixCalls  = 0;
    int inverseCalls = 0;
    int volumeCalls  = 0;

    double matrixFn(const double& dim, const double& angle)
    {
        ++matrixCalls;
        return dim * angle;
    }

    double inverseFn(const double& matrix)
    {
        ++inverseCalls;
        return 1.0 / matrix;
    }

    double volumeFn(const double& matrix)
    {
        ++volumeCalls;
        return matrix * 2.0;
    }

    struct MiniBox
    {
        Length dim{2.0};
        Length angle{3.0};

        mstd::MemberCache<double, MiniBox, &MiniBox::dim, &MiniBox::angle>
                                                             matrix{matrixFn};
        mstd::MemberCache<double, MiniBox, &MiniBox::matrix> inverse{inverseFn};
        mstd::MemberCache<double, MiniBox, &MiniBox::matrix> volume{volumeFn};
    };
}   // namespace

TEST_CASE("MemberCache computes lazily and only once", "[types][member_cache]")
{
    resetCalls();
    Shape s;

    REQUIRE(areaCalls == 0);   // nothing computed on construction

    REQUIRE(s.getArea() == 6.0);
    REQUIRE(s.getArea() == 6.0);
    REQUIRE(s.getArea() == 6.0);
    REQUIRE(areaCalls == 1);
}

TEST_CASE(
    "MemberCache recomputes after a source changed",
    "[types][member_cache]"
)
{
    resetCalls();
    Shape s;
    REQUIRE(s.getArea() == 6.0);

    s.a.set(5.0);
    REQUIRE(s.getArea() == 15.0);
    REQUIRE(areaCalls == 2);

    s.a.set(1.0);
    s.b.set(1.0);   // two writes, still one recompute on the next read
    REQUIRE(s.getArea() == 1.0);
    REQUIRE(areaCalls == 3);
}

TEST_CASE(
    "A write in another domain does not invalidate",
    "[types][member_cache]"
)
{
    resetCalls();
    Shape s;
    REQUIRE(s.getArea() == 6.0);

    s.v.set(99.0);
    REQUIRE(s.getArea() == 6.0);
    REQUIRE(areaCalls == 1);
}

TEST_CASE("A cache can depend on another cache", "[types][member_cache]")
{
    resetCalls();
    Shape s;

    REQUIRE(s.getPlus() == 7.0);
    REQUIRE(s.getPlus() == 7.0);
    REQUIRE(areaCalls == 1);
    REQUIRE(plusCalls == 1);

    s.v.set(1.0);   // other domain: nothing recomputes
    REQUIRE(s.getPlus() == 7.0);
    REQUIRE(areaCalls == 1);
    REQUIRE(plusCalls == 1);

    s.a.set(4.0);   // the change propagates through the chain
    REQUIRE(s.getPlus() == 13.0);
    REQUIRE(areaCalls == 2);
    REQUIRE(plusCalls == 2);
}

TEST_CASE(
    "Reading the dependent cache fills the source cache",
    "[types][member_cache]"
)
{
    resetCalls();
    Shape s;

    REQUIRE(s.getPlus() == 7.0);   // computes area on the way
    REQUIRE(areaCalls == 1);

    REQUIRE(s.getArea() == 6.0);   // hit: already filled
    REQUIRE(areaCalls == 1);
}

TEST_CASE(
    "A chain over two domains recomputes only what changed",
    "[types][member_cache]"
)
{
    resetCalls();
    Shape s;

    REQUIRE(s.getScaled() == 60.0);
    REQUIRE(areaCalls == 1);
    REQUIRE(scaledCalls == 1);

    s.v.set(2.0);   // only the outer cache is affected
    REQUIRE(s.getScaled() == 12.0);
    REQUIRE(areaCalls == 1);
    REQUIRE(scaledCalls == 2);

    s.b.set(5.0);   // both are affected
    REQUIRE(s.getScaled() == 20.0);
    REQUIRE(areaCalls == 2);
    REQUIRE(scaledCalls == 3);
}

TEST_CASE(
    "An owner keeps defaulted copy, move and assignment",
    "[types][member_cache]"
)
{
    Shape s;
    REQUIRE(s.getPlus() == 7.0);

    Shape copy{s};
    REQUIRE(copy.getPlus() == 7.0);

    copy.a.set(10.0);
    REQUIRE(copy.getPlus() == 31.0);   // follows the copy's own property
    REQUIRE(s.getPlus() == 7.0);       // original unaffected

    s = copy;   // defaulted assignment
    REQUIRE(s.getPlus() == 31.0);
    s.b.set(1.0);
    REQUIRE(s.getArea() == 10.0);
    REQUIRE(copy.getArea() == 30.0);

    Shape moved{std::move(s)};
    REQUIRE(moved.getArea() == 10.0);

    Shape target;
    target = std::move(moved);
    REQUIRE(target.getArea() == 10.0);
}

TEST_CASE("Sources can be members of a base class", "[types][member_cache]")
{
    Derived d;
    REQUIRE(d.getHalf() == 2.0);

    d.setLength(10.0);
    REQUIRE(d.getHalf() == 5.0);
}

TEST_CASE("A throwing compute keeps the cache invalid", "[types][member_cache]")
{
    Fragile f;
    REQUIRE(f.value.get(f) == 1.0);

    f.x.set(-1.0);
    REQUIRE_THROWS_AS(f.value.get(f), std::runtime_error);
    REQUIRE_THROWS_AS(f.value.get(f), std::runtime_error);   // not served stale

    f.x.set(2.0);
    REQUIRE(f.value.get(f) == 2.0);
}

TEST_CASE(
    "A MemberCache without compute function throws",
    "[types][member_cache]"
)
{
    struct Holder
    {
        Length                                        x{1.0};
        mstd::MemberCache<double, Holder, &Holder::x> value{nullptr};
    };

    Holder h;
    REQUIRE_THROWS_AS(h.value.get(h), mstd::RuntimeError);
}

TEST_CASE(
    "Box like chain: matrix once, inverse only on demand",
    "[types][member_cache]"
)
{
    matrixCalls  = 0;
    inverseCalls = 0;
    volumeCalls  = 0;

    MiniBox box;

    REQUIRE(box.volume.get(box) == 12.0);   // fills the matrix cache
    REQUIRE(matrixCalls == 1);
    REQUIRE(volumeCalls == 1);
    REQUIRE(inverseCalls == 0);   // never asked for

    REQUIRE(box.matrix.get(box) == 6.0);   // hit
    REQUIRE(matrixCalls == 1);

    REQUIRE(box.inverse.get(box) == 1.0 / 6.0);
    REQUIRE(matrixCalls == 1);   // reuses the cached matrix
    REQUIRE(inverseCalls == 1);

    box.dim.set(4.0);   // everything is stale
    REQUIRE(box.volume.get(box) == 24.0);
    REQUIRE(matrixCalls == 2);
    REQUIRE(inverseCalls == 1);   // still not recomputed
    REQUIRE(box.inverse.get(box) == 1.0 / 12.0);
    REQUIRE(matrixCalls == 2);
    REQUIRE(inverseCalls == 2);

    MiniBox copy{box};   // defaulted copy
    copy.angle.set(1.0);
    REQUIRE(copy.volume.get(copy) == 8.0);
    REQUIRE(box.volume.get(box) == 24.0);
}
