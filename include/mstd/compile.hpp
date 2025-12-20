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

#ifndef __MSTD__COMPILE_HPP__
#define __MSTD__COMPILE_HPP__

/**
 * @brief Ignore buggy code
 *
 * Define this macro to ignore code that is known to be buggy in certain
 * environments/libraries/compilers.
 *
 * @note at the moment the exact value of this macro does not matter
 * as long as it is defined or not defined
 *
 */
#ifdef MSTD_IGNORE_BUGGY_CODE
#undef MSTD_IGNORE_BUGGY_CODE
#define MSTD_IGNORE_BUGGY_CODE 1
#else
#define MSTD_IGNORE_BUGGY_CODE 0
#endif

#endif   // __MSTD__COMPILE_HPP__