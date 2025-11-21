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

"""Module defining enumerations used in C++ checks."""

from enum import Enum


class FileType(Enum):
    """Enumeration of file types for C++ checks."""

    UNKNOWN = 0
    CPPHeader = 1
    CPPSource = 2
    CMakeLists = 3


class ResultTypeEnum(Enum):
    """Enumeration of result types for C++ type checks."""

    Ok = 0
    Warning = 1
    Error = 2
