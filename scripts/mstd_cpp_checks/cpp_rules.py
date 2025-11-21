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

from cpp_types import ResultType
from enums import FileType, check_key_sequence_ordered


def static_constexpr_inline_rule(line: str) -> ResultType:
    """Check for usage of static, constexpr, and inline in variable declarations.

    Parameters
    ----------
    line: str
        The line of code to check.

    Returns
    -------
    cpp_types.ResultType:
        Result of the check, Warning if all keywords are not present, Ok otherwise

    """
    key_sequence = ["static", "inline", "constexpr"]
    return check_key_sequence_ordered(key_sequence, line)


cpp_rules = []
cpp_rules.append(
    [
        {FileType.CPPHeader, FileType.CPPSource},
        static_constexpr_inline_rule
    ]
)
