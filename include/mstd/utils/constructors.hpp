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

#ifndef __MSTD__UTILS__CONSTRUCTORS_HPP__
#define __MSTD__UTILS__CONSTRUCTORS_HPP__

#define MSTD_NO_COPY(className)                      \
    className(const className&)            = delete; \
    className& operator=(const className&) = delete;

#define MSTD_DEFAULT_COPY(className)                  \
    className(const className&)            = default; \
    className& operator=(const className&) = default;

#define MSTD_NO_MOVE(className)                 \
    className(className&&)            = delete; \
    className& operator=(className&&) = delete;

#define MSTD_DEFAULT_MOVE(className)             \
    className(className&&)            = default; \
    className& operator=(className&&) = default;

#define MSTD_DEFAULT_DESTRUCTORS(className) ~className() = default;

#define MSTD_DEFAULT_CONSTRUCTORS(className) \
    MSTD_DEFAULT_COPY(className)             \
    MSTD_DEFAULT_MOVE(className)             \
    MSTD_DEFAULT_DESTRUCTORS(className)

#define MSTD_RAII_DEFAULT_MOVE(className) \
    MSTD_NO_COPY(className)               \
    MSTD_DEFAULT_MOVE(className)

#define MSTD_RAII_NO_MOVE(className) \
    MSTD_NO_COPY(className)          \
    MSTD_NO_MOVE(className)

#endif   // __MSTD__UTILS__CONSTRUCTORS_HPP__
