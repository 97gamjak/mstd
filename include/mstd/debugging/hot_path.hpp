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

// hot_path.hpp
#include "mstd/exceptions.hpp"

#if defined(MSTD_DEBUG) && !defined(MSTD_PERF_CHECKS)
#define MSTD_PERF_CHECKS
#endif

namespace mstd
{
#ifdef MSTD_PERF_CHECKS
    class PerformanceError : public RuntimeError
    {
       public:
        using RuntimeError::RuntimeError;
    };

    namespace detail
    {
        struct HotPathState
        {
            int depth  = 0;   // nested hot scopes on this thread
            int strict = 0;   // how many of them are strict
        };
        inline thread_local HotPathState hotPath;
    }   // namespace detail

    class HotPath
    {
        bool _strict;

       public:
        explicit HotPath(bool strict = false) noexcept : _strict(strict)
        {
            ++detail::hotPath.depth;
            if (_strict)
                ++detail::hotPath.strict;
        }
        ~HotPath()
        {
            --detail::hotPath.depth;
            if (_strict)
                --detail::hotPath.strict;
        }
        HotPath(const HotPath&)            = delete;
        HotPath& operator=(const HotPath&) = delete;
    };

    /// writes and recomputes: forbidden in any hot scope
    inline void assertNotHot(const char* what)
    {
        if (detail::hotPath.depth > 0)
            throw PerformanceError(std::string(what) + " inside a hot path");
    }

    /// cache hits: forbidden only in strict scopes (hoist the read above the
    /// loop)
    inline void assertNotStrict(const char* what)
    {
        if (detail::hotPath.strict > 0)
            throw PerformanceError(
                std::string(what) + " inside a strict hot path"
            );
    }

#define MSTD_HOT_PATH_CAT2(a, b) a##b
#define MSTD_HOT_PATH_CAT(a, b)  MSTD_HOT_PATH_CAT2(a, b)
#define MSTD_HOT_PATH \
    const ::mstd::HotPath MSTD_HOT_PATH_CAT(mstdHot_, __LINE__) {}
#define MSTD_HOT_PATH_STRICT \
    const ::mstd::HotPath MSTD_HOT_PATH_CAT(mstdHot_, __LINE__) { true }
#define MSTD_ASSERT_NOT_HOT(w)    ::mstd::assertNotHot(w)
#define MSTD_ASSERT_NOT_STRICT(w) ::mstd::assertNotStrict(w)

#else   // release: everything vanishes

#define MSTD_HOT_PATH
#define MSTD_HOT_PATH_STRICT
#define MSTD_ASSERT_NOT_HOT(w)    ((void) 0)
#define MSTD_ASSERT_NOT_STRICT(w) ((void) 0)

#endif
}   // namespace mstd
