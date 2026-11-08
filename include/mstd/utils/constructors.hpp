#ifndef __MSTD__UTILS__CONSTRUCTORS_HPP__
#define __MSTD__UTILS__CONSTRUCTORS_HPP__

#define MSTD_RAII_CLASS(className)                    \
    className(const className&)            = delete;  \
    className& operator=(const className&) = delete;  \
    className(className&&)                 = default; \
    className& operator=(className&&)      = default;

#endif   // __MSTD__UTILS__CONSTRUCTORS_HPP__
